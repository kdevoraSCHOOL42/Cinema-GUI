#include "DialogAddSession.h"
#include "ui_DialogAddSession.h"

DialogAddSession::DialogAddSession(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::DialogAddSession) {
    ui->setupUi(this);
}

DialogAddSession::~DialogAddSession() {
    delete ui;
}

std::tm DialogAddSession::getDateTime() const {
    std::tm tm = {};
    tm.tm_year = ui->spinBoxYear->value() - 1900;
    tm.tm_mon = ui->spinBoxMonth->value() - 1;
    tm.tm_mday = ui->spinBoxDay->value();
    tm.tm_hour = ui->spinBoxHour->value();
    tm.tm_min = ui->spinBoxMinute->value();
    tm.tm_sec = 0;
    return tm;
}
