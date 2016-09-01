#-------------------------------------------------
#
# Project created by QtCreator 2016-07-15T16:14:46
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = karabiner
TEMPLATE = app
LIBS += -L../lib/ -lglbcrypto


SOURCES += \
    src/main.cpp \
    src/mainwindow.cpp \
    src/newdialog.cpp \
    src/user.cpp \
    src/userdata.cpp \
    src/pwentry.cpp

HEADERS += \
    inc/mainwindow.h \
    inc/userdata.h \
    inc/user.h \
    inc/newdialog.h \
    inc/glbcrypto/misc.h \
    inc/glbcrypto/aes.h \
    inc/glbcrypto/sha.h \
    inc/pwentry.h


FORMS += \
    ui/mainwindow.ui

RESOURCES += \
    res.qrc
