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
    voicecenter.cpp \
    ../share_library/messagecenter.cpp \
    ../share_library/Util.cpp

HEADERS += \
    voicecenter.h \
    ../share_library/messagecenter.h \
    ../share_library/Util.h
