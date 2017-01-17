QT += core gui widgets
QT += network
CONFIG += c++11

TARGET = ThreadMatrixClient
CONFIG -= app_bundle

TEMPLATE = app

SOURCES += main.cpp \
    mainwindow.cpp

HEADERS += \
    mainwindow.h

FORMS += \
    mainwindow.ui
