#-------------------------------------------------
#
# Project created by QtCreator 2014-12-26T11:46:34
#
#-------------------------------------------------

QT       += core
QT += widgets network
QT       -= gui

TARGET = zhanqi_guaji
CONFIG   += console
CONFIG   -= app_bundle
CONFIG   += c++11
TEMPLATE = app


SOURCES += main.cpp \
    ../tcp_socket_test/danmuconnection.cpp \
    ../share_library/httpfiledownloader.cpp \
    ../share_library/messagecenter.cpp \
    ../share_library/Util.cpp \
    ../share_library/zhanqiutil.cpp

HEADERS += \
    ../tcp_socket_test/danmuconnection.h \
    ../share_library/httpfiledownloader.h \
    ../share_library/messagecenter.h \
    ../share_library/Util.h \
    ../share_library/zhanqiutil.h

OTHER_FILES += \
    1.txt
