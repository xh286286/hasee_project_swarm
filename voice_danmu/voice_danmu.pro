#-------------------------------------------------
#
# Project created by QtCreator 2014-11-09T16:13:09
#
#-------------------------------------------------

QT       += core gui network
QT += network \
      xml \
      multimedia \
      multimediawidgets \
      widgets
CONFIG += c++11

TARGET = voice_danmu
CONFIG   += console
CONFIG   -= app_bundle

TEMPLATE = app


SOURCES += main.cpp \
    messagecenter.cpp \
    voicecenter.cpp \
    Util.cpp

HEADERS += \
    messagecenter.h \
    voicecenter.h \
    Util.h
