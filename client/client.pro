#-------------------------------------------------
#
# Project created by QtCreator 2017-10-09T23:11:14
#
#-------------------------------------------------

QT       += core gui network

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = client
TEMPLATE = app

DEFINES += QT_DEPRECATED_WARNINGS

SOURCES += main.cpp\
        mainwindow.cpp \
        ../common/command.cpp \
        dialog.cpp

HEADERS  += mainwindow.h \
        ../common/command.h \
        dialog.h
