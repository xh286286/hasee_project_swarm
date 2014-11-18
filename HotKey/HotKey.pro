#-------------------------------------------------
#
# Project created by QtCreator 2013-12-23T17:04:37
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = HotKey
TEMPLATE = app


SOURCES += main.cpp\
        Widget.cpp \
    CHotKeyEventFilter.cpp

HEADERS  += Widget.hpp \
    CHotKeyEventFilter.hpp
