#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "DialogAddMovie.h"
#include "DialogAddSession.h"
#include <QMessageBox>
#include <QFileDialog>
#include <QJsonDocument>
#include <QJsonArray>
#include <QJsonObject>
#include <QDateTime>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent), ui(new Ui::MainWindow), cinema(10) {
    ui->setupUi(this);
    movieModel = new MovieTableModel(&cinema, this);
    sessionModel = new SessionTableModel(this);

    ui->tableViewMovies->setModel(movieModel);
    ui->tableViewSessions->setModel(sessionModel);

    setupConnections();
}

MainWindow::~MainWindow() {
    delete ui;
}

void MainWindow::setupConnections() {
    connect(ui->btnAddMovie, &QPushButton::clicked, this, &MainWindow::onAddMovie);
    connect(ui->btnRemoveMovie, &QPushButton::clicked, this, &MainWindow::onRemoveMovie);
    connect(ui->btnAddSession, &QPushButton::clicked, this, &MainWindow::onAddSession);
    connect(ui->btnRemoveSession, &QPushButton::clicked, this, &MainWindow::onRemoveSession);
    connect(ui->btnSave, &QPushButton::clicked, this, &MainWindow::onSaveToFile);
    connect(ui->btnLoad, &QPushButton::clicked, this, &MainWindow::onLoadFromFile);
    connect(ui->tableViewMovies->selectionModel(), &QItemSelectionModel::currentRowChanged,
            this, &MainWindow::onMovieSelected);
}

void MainWindow::onAddMovie() {
    DialogAddMovie dlg(this);
    if (dlg.exec() == QDialog::Accepted) {
        Movie m(dlg.getTitle(), dlg.getDuration());
        cinema.addMovie(std::move(m));
        movieModel->layoutChanged();
    }
}

void MainWindow::onRemoveMovie() {
    QModelIndex index = ui->tableViewMovies->currentIndex();
    if (!index.isValid()) return;
    cinema.removeMovie(index.row());
    movieModel->layoutChanged();
    sessionModel->setMovie(nullptr);
}

void MainWindow::onMovieSelected(const QModelIndex &index) {
    if (!index.isValid()) return;
    Movie* m = cinema.getMovieAt(index.row());
    sessionModel->setMovie(m);
}

void MainWindow::onAddSession() {
    QModelIndex index = ui->tableViewMovies->currentIndex();
    if (!index.isValid()) return;
    Movie* m = cinema.getMovieAt(index.row());
    DialogAddSession dlg(this);
    if (dlg.exec() == QDialog::Accepted) {
        m->addSessionToSchedule(dlg.getDateTime());
        movieModel->layoutChanged();
        sessionModel->layoutChanged();
    }
}

void MainWindow::onRemoveSession() {
    QModelIndex movieIndex = ui->tableViewMovies->currentIndex();
    QModelIndex sessionIndex = ui->tableViewSessions->currentIndex();
    if (!movieIndex.isValid() || !sessionIndex.isValid()) return;
    Movie* m = cinema.getMovieAt(movieIndex.row());
    m->removeSessionAt(sessionIndex.row());
    movieModel->layoutChanged();
    sessionModel->layoutChanged();
}

void MainWindow::onSaveToFile() {
    QString fileName = QFileDialog::getSaveFileName(this, "Сохранить файл", "", "JSON (*.json)");
    if (fileName.isEmpty()) return;
    QFile file(fileName);
    if (!file.open(QIODevice::WriteOnly)) return;

    QJsonArray movieArray;
    for (int i = 0; i < cinema.getMovieCount(); ++i) {
        Movie* m = cinema.getMovieAt(i);
        QJsonObject movieObj;
        movieObj["title"] = QString::fromStdString(m->getTitle());
        movieObj["duration"] = m->getDurationMinutes();

        QJsonArray sessions;
        for (const auto& tm : m->getSessionTimes()) {
            QDateTime dt = QDateTime::fromSecsSinceEpoch(std::mktime(const_cast<std::tm*>(&tm)));
            sessions.append(dt.toString(Qt::ISODate));
        }
        movieObj["sessions"] = sessions;
        movieArray.append(movieObj);
    }

    QJsonDocument doc(movieArray);
    file.write(doc.toJson());
    file.close();
}

void MainWindow::onLoadFromFile() {
    QString fileName = QFileDialog::getOpenFileName(this, "Открыть файл", "", "JSON (*.json)");
    if (fileName.isEmpty()) return;
    QFile file(fileName);
    if (!file.open(QIODevice::ReadOnly)) return;

    cinema.clear();
    QJsonDocument doc = QJsonDocument::fromJson(file.readAll());
    QJsonArray movieArray = doc.array();

    for (const QJsonValue& val : movieArray) {
        QJsonObject obj = val.toObject();
        Movie m(obj["title"].toString().toStdString(), obj["duration"].toInt());

        QJsonArray sessions = obj["sessions"].toArray();
        for (const QJsonValue& s : sessions) {
            QDateTime dt = QDateTime::fromString(s.toString(), Qt::ISODate);
            std::tm tm = *std::localtime(&(time_t){dt.toSecsSinceEpoch()});
            m.addSessionToSchedule(tm);
        }
        cinema.addMovie(std::move(m));
    }

    movieModel->layoutChanged();
    sessionModel->setMovie(nullptr);
    sessionModel->layoutChanged();
}
