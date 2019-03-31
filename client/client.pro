#-------------------------------------------------
#
# Project created by QtCreator 2017-10-09T23:11:14
#
#-------------------------------------------------

QT       += core gui network

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = client
TEMPLATE = app

CONFIG += c++11

DEFINES += QT_DEPRECATED_WARNINGS

SOURCES += \
         main.cpp\
        mainwindow.cpp \
        ../common/command.cpp \
        ../common/lobby.cpp \
        ../common/player.cpp \
        dialog.cpp \
    coridorwindow.cpp \
    images.cpp \
    field.cpp \
    quartowindow.cpp \
    coridorlogic.cpp \
    quartologic.cpp \
    dialogchoseplayer.cpp

HEADERS  += \
        mainwindow.h \
        ../common/command.h \
        ../common/lobby.h \
        ../common/player.h \
        dialog.h \
    coridorwindow.h \
    images.h \
    field.h \
    quartowindow.h \
    coridorlogic.h \
    quartologic.h \
    dialogchoseplayer.h

FORMS += \
    coridorwindow.ui \
    quartowindow.ui
