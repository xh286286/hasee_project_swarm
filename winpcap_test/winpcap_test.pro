#-------------------------------------------------
#
# Project created by QtCreator 2014-09-02T16:21:17
#
#-------------------------------------------------



QT       += core gui network
QT += widgets
TARGET = winpcap_test

CONFIG += console
CONFIG   -= app_bundle
CONFIG += c++11
TEMPLATE = app

INCLUDEPATH += D:/weiyun_tongbu/project/WpdPack_4_1_2/WpdPack/Include
LIBS += D:/weiyun_tongbu/project/WpdPack_4_1_2/WpdPack/Lib/wpcap.lib

SOURCES += main.cpp \
    workerthread.cpp \
    danmuwindow.cpp \
    danmu.cpp \
    danmustate.cpp \
    windowsizesetting.cpp \
    mymenu.cpp \
    ../share_library/httpfiledownloader.cpp \
    ../share_library/messagecenter.cpp \
    ../share_library/Util.cpp \
    ../tcp_socket_test/danmuconnection.cpp \
    ../share_library/zhanqiutil.cpp \
    autothankgift.cpp

HEADERS += \
    workerthread.h \
    danmuwindow.h \
    danmu.h \
    danmustate.h \
    windowsizesetting.h \
    mymenu.h \
    ../share_library/httpfiledownloader.h \
    ../share_library/messagecenter.h \
    ../share_library/Util.h \
    ../tcp_socket_test/danmuconnection.h \
    ../share_library/zhanqiutil.h \
    autothankgift.h

OTHER_FILES += \
    parse.txt \
    ../share_library/share_library.pro.user

RESOURCES += \
    ziyuan.qrc

SUBDIRS += \
    ../share_library/share_library.pro
