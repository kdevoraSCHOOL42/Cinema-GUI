QT += core gui
greaterThan(QT_MAJOR_VERSION, 4): QT += widgets
TARGET = CinemaGUI
TEMPLATE = app

SOURCES += main.cpp     mainwindow.cpp     Movie.cpp     MovieSession.cpp     SessionSchedule.cpp     CinemaSchedule.cpp     MovieTableModel.cpp     SessionTableModel.cpp     DialogAddMovie.cpp     DialogAddSession.cpp

HEADERS += mainwindow.h     Movie.h     MovieSession.h     SessionSchedule.h     CinemaSchedule.h     MovieTableModel.h     SessionTableModel.h     DialogAddMovie.h     DialogAddSession.h

FORMS += mainwindow.ui     DialogAddMovie.ui     DialogAddSession.ui
