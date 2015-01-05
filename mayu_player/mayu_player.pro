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
 

CONFIG += console
CONFIG -= app_bundle
TARGET = mayu_player
TEMPLATE = app
 

RESOURCES += \
    img.qrc

OTHER_FILES += \
    test.txt

SOURCES += \
    main.cpp \
    videowidget.cpp \
    userbank.cpp \
    playlistmodel.cpp \
    playercontrols.cpp \
    player2.cpp \
    player.cpp \
    mydialog.cpp \
    histogramwidget.cpp \
    CHotKeyEventFilter.cpp \
    ../share_library/httpfiledownloader.cpp \
    ../share_library/messagecenter.cpp \
    ../share_library/Util.cpp \
    ../share_library/zhanqiutil.cpp

HEADERS += \
    videowidget.h \
    userbank.h \
    playlistmodel.h \
    playercontrols.h \
    player.h \
    mydialog.h \
    histogramwidget.h \
    CHotKeyEventFilter.hpp \
    ../share_library/httpfiledownloader.h \
    ../share_library/messagecenter.h \
    ../share_library/Util.h \
    ../share_library/zhanqiutil.h

SUBDIRS += \
    ../share_library/share_library.pro


