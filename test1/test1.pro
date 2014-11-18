#-------------------------------------------------
#
# Project created by QtCreator 2014-10-05T17:10:10
#
#-------------------------------------------------

QT       += core gui network
QT += network \
      xml \
      multimedia \
      multimediawidgets \
      widgets
CONFIG += c++11
greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += console
CONFIG -= app_bundle
TARGET = test1
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
    mydialog.cpp \
    messagecenter.cpp \
    httpfiledownloader.cpp \
    histogramwidget.cpp \
    player.cpp \
    playercontrols.cpp \
    playlistmodel.cpp \
    videowidget.cpp \
    Util.cpp \
    userbank.cpp \
    CHotKeyEventFilter.cpp \
    player2.cpp

HEADERS  += mainwindow.h \
    mydialog.h \
    messagecenter.h \
    httpfiledownloader.h \
    histogramwidget.h \
    player.h \
    playercontrols.h \
    playlistmodel.h \
    videowidget.h \
    Util.h \
    userbank.h \
    CHotKeyEventFilter.hpp

FORMS    += mainwindow.ui

OTHER_FILES += \
    test1.pro.user \
    test.txt \
    test.js \
    test2.js

RESOURCES += \
    img.qrc
