#-------------------------------------------------
#
# Project created by QtCreator 2015-07-03T11:25:07
#
#-------------------------------------------------

QT       += core gui
QT += serialport
CONFIG += c++11

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = Multimeter
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
    settingsdialog.cpp

HEADERS  += mainwindow.h \
    settingsdialog.h

FORMS    += mainwindow.ui \
    settingsdialog.ui
