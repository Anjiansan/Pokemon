#-------------------------------------------------
#
# Project created by QtCreator 2016-10-08T15:41:17
#
#-------------------------------------------------

QT       += core gui network sql testlib

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = Pokemon_Server
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
    pokemon.cpp \
    highattack.cpp \
    highblood.cpp \
    highdefense.cpp \
    highspeed.cpp

HEADERS  += mainwindow.h \
    pokemon.h \
    highattack.h \
    highblood.h \
    highdefense.h \
    highspeed.h

FORMS    += mainwindow.ui
