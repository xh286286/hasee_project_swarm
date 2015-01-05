#-------------------------------------------------
#
# Project created by QtCreator 2014-11-09T16:13:09
#
#-------------------------------------------------

QT       += core gui widgets
QT += multimedia
TARGET = mayu_voice_danmu

CONFIG += console
CONFIG   -= app_bundle
CONFIG += c++11
TEMPLATE = app
HEADERS += \
    voicecenter.h \
    ../share_library/httpfiledownloader.h \
    ../share_library/messagecenter.h \
    ../share_library/Util.h \
    ../share_library/zhanqiutil.h

SOURCES += \
    main.cpp \
    voicecenter.cpp \
    ../share_library/httpfiledownloader.cpp \
    ../share_library/messagecenter.cpp \
    ../share_library/Util.cpp \
    ../share_library/zhanqiutil.cpp


