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
    ../share_library/httpfiledownloader.cpp \
    ../share_library/zhanqiutil.cpp \
    ../share_library/Util.cpp \
    zhanqihttpconnection.cpp

HEADERS += \
    danmuconnection.h \
    ../share_library/httpfiledownloader.h \
    ../share_library/zhanqiutil.h \
    ../share_library/Util.h \
    zhanqihttpconnection.h

OTHER_FILES += \
    connect.txt

DISTFILES += \
    back.txt \
    connection.txt \
    http.txt \
    ../zhanqi_video_record/room_info.txt
