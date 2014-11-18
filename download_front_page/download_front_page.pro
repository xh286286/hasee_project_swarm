#-------------------------------------------------
#
# Project created by QtCreator 2014-11-18T13:42:08
#
#-------------------------------------------------

QT       += core gui network

QT += network \
      xml \
      multimedia \
      multimediawidgets \
      widgets

TARGET = download_front_page
CONFIG   += console
CONFIG   -= app_bundle

TEMPLATE = app


SOURCES += main.cpp \
    ../share_library/httpfiledownloader.cpp


HEADERS += \
    ../share_library/httpfiledownloader.h
