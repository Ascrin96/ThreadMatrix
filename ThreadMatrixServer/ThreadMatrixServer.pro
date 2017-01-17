QT += core
QT += network
QT -= gui

CONFIG += c++11 -lpthread

TARGET = ThreadMatrixServer
CONFIG += console
CONFIG -= app_bundle

TEMPLATE = app

SOURCES += main.cpp \
    mytcpserver.cpp \
    threadmat.cpp

HEADERS += \
    mytcpserver.h \
    threadmat.h
