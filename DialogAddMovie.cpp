#include "DialogAddMovie.h"
#include "ui_DialogAddMovie.h"

DialogAddMovie::DialogAddMovie(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::DialogAddMovie) {
    ui->setupUi(this);
}

DialogAddMovie::~DialogAddMovie() {
    delete ui;
}

QString DialogAddMovie::getTitle() const {
    return ui->lineEditTitle->text();
}

int DialogAddMovie::getDuration() const {
    return ui->spinBoxDuration->value();
}
