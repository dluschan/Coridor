QT += core
QT -= gui
QT +=network

CONFIG += c++11

TARGET = server
CONFIG += console
CONFIG -= app_bundle

TEMPLATE = app

SOURCES += main.cpp \
    myserver.cpp

HEADERS += \
    myserver.h

DEFINES += QT_DEPRECATED_WARNINGS
