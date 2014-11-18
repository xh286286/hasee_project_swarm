#-------------------------------------------------
#
# Project created by QtCreator 2014-11-11T16:14:43
#
#-------------------------------------------------

QT       += core gui network

QT += network \
      xml \
      multimedia \
      multimediawidgets \
      widgets
TARGET = tcp_socket_test
CONFIG   += console
CONFIG += c++11
CONFIG   -= app_bundle

TEMPLATE = app


SOURCES += main.cpp \
    danmuconnection.cpp \
    httpfiledownloader.cpp

HEADERS += \
    danmuconnection.h \
    httpfiledownloader.h

OTHER_FILES += \
    connect.txt
