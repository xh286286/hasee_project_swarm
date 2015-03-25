#-------------------------------------------------
#
# Project created by QtCreator 2014-11-18T16:09:47
#
#-------------------------------------------------

QT       += core network widgets

QT       -= gui

TARGET = share_library
CONFIG   += console c++11
CONFIG   -= app_bundle

TEMPLATE = app


SOURCES += main.cpp \
    httpfiledownloader.cpp \
    messagecenter.cpp \
    Util.cpp \
    zhanqiutil.cpp \
    xhdebuglog.cpp

HEADERS += \
    httpfiledownloader.h \
    messagecenter.h \
    Util.h \
    zhanqiutil.h \
    xhdebuglog.h
