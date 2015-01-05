#-------------------------------------------------
#
# Project created by QtCreator 2014-10-05T17:10:10
#
#-------------------------------------------------

QT       += core gui
QT += network \
      xml \
      multimedia \
      multimediawidgets \
      widgets
CONFIG += c++11
greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += console
CONFIG -= app_bundle
TARGET = mayu_player
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
    mydialog.cpp \
    histogramwidget.cpp \
    player.cpp \
    playercontrols.cpp \
    playlistmodel.cpp \
    videowidget.cpp \
    userbank.cpp \
    CHotKeyEventFilter.cpp \
    player2.cpp \
    ../share_library/httpfiledownloader.cpp \
    ../share_library/messagecenter.cpp \
    ../share_library/Util.cpp \
    ../share_library/zhanqiutil.cpp

HEADERS  += mainwindow.h \
    mydialog.h \
    histogramwidget.h \
    player.h \
    playercontrols.h \
    playlistmodel.h \
    videowidget.h \
    userbank.h \
    CHotKeyEventFilter.hpp \
    ../share_library/httpfiledownloader.h \
    ../share_library/messagecenter.h \
    ../share_library/Util.h \
    ../share_library/zhanqiutil.h

FORMS    += mainwindow.ui

OTHER_FILES += \
    test.txt \
    test.js \
    test2.js

RESOURCES += \
    img.qrc
