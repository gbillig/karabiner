#-------------------------------------------------
#
# Project created by QtCreator 2016-07-15T16:14:46
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = karabiner-gui
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
    newdialog.cpp \
    userdata.cpp \
    user.cpp

HEADERS  += mainwindow.h \
    newdialog.h \
    userdata.h \
    user.h

FORMS    += mainwindow.ui

RESOURCES += \
    res.qrc
