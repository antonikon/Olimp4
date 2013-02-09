#-------------------------------------------------
#
# Project created by QtCreator 2013-01-06T19:53:22
#
#-------------------------------------------------

QT       += core gui network

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = Olimp4
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
    loader.cpp \
    cJSON.c

HEADERS  += mainwindow.h \
    loader.h \
    cJSON.h

FORMS    += mainwindow.ui

QMAKE_CXXFLAGS += -std=c++0x
