#-------------------------------------------------
#
# Project created by QtCreator 2014-09-02T16:21:17
#
#-------------------------------------------------



QT       += core gui
QT += widgets
TARGET = winpcap_test

CONFIG += console
CONFIG   -= app_bundle

TEMPLATE = app

INCLUDEPATH += D:/git_qt/WpdPack_4_1_2/WpdPack/Include
LIBS += D:/git_qt/WpdPack_4_1_2/WpdPack/Lib/wpcap.lib
LIBS += D:\git_qt\WpdPack_4_1_2\WpdPack\Lib\ws2_32\WS2_32.Lib
SOURCES += main.cpp \
    workerthread.cpp \
    danmuwindow.cpp \
    danmu.cpp \
    Util.cpp \
    danmustate.cpp \
    windowsizesetting.cpp \
    mymenu.cpp \
    messagecenter.cpp

HEADERS += \
    workerthread.h \
    danmuwindow.h \
    danmu.h \
    Util.h \
    danmustate.h \
    windowsizesetting.h \
    mymenu.h \
    messagecenter.h

OTHER_FILES += \
    parse.txt

RESOURCES += \
    ziyuan.qrc
