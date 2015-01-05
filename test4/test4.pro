#-------------------------------------------------
#
# Project created by QtCreator 2014-12-25T16:04:20
#
#-------------------------------------------------

QT       += core
QT -= gui
QT += widgets network

TARGET = test4
CONFIG   += console
CONFIG   -= app_bundle

TEMPLATE = app


SOURCES += main.cpp \
    ../share_library/httpfiledownloader.cpp \
    ../share_library/Util.cpp \
    ../share_library/zhanqiutil.cpp \
    ../tcp_socket_test/danmuconnection.cpp

HEADERS += \
    ../share_library/httpfiledownloader.h \
    ../share_library/Util.h \
    ../share_library/zhanqiutil.h \
    ../tcp_socket_test/danmuconnection.h
