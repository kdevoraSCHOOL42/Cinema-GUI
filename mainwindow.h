#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "CinemaSchedule.h"
#include "MovieTableModel.h"
#include "SessionTableModel.h"

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow {
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void onAddMovie();
    void onRemoveMovie();
    void onMovieSelected(const QModelIndex &index);
    void onAddSession();
    void onRemoveSession();
    void onSaveToFile();
    void onLoadFromFile();

private:
    Ui::MainWindow *ui;
    CinemaSchedule cinema;
    MovieTableModel *movieModel;
    SessionTableModel *sessionModel;

    void setupConnections();
    void refreshSessionTable();
};

#endif // MAINWINDOW_H
