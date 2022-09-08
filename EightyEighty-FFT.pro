#-------------------------------------------------
#
# Project created by QtCreator 2015-10-31T22:56:00
#
#-------------------------------------------------

QT       += core gui network

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = EightyEighty-FFT
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
    transfers.cpp \
    tsend.cpp \
    treceive.cpp

HEADERS  += mainwindow.h \
    transfers.h \
    tsend.h \
    treceive.h

FORMS    += mainwindow.ui
