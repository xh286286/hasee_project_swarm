#-------------------------------------------------
#
# Project created by QtCreator 2014-04-21T08:51:48
#
#-------------------------------------------------

QT       += core gui network

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = sisvisit

CONFIG   += c++11
CONFIG  += console
CONFIG   += warn_on
CONFIG   -= app_bundle

QMAKE_CXXFLAGS += -Wall

TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
    util.cpp \
    mymutexlocker.cpp \
    sisthread.cpp \
    jasonobject.cpp \
    authorset.cpp \
    refercenter.cpp \
    filecenter.cpp \
    keywordset.cpp \
    httpproxy.cpp \
    networkutil.cpp \
    poster.cpp

HEADERS  += mainwindow.h \
    util.h \
    mymutexlocker.h \
    sisthread.h \
    jasonobject.h \
    authorset.h \
    refercenter.h \
    filecenter.h \
    keywordset.h \
    httpproxy.h \
    networkutil.h \
    poster.h

FORMS    += mainwindow.ui

OTHER_FILES += \
    trashcode.txt \
    design.txt \
    proxy.ini \
    save.ini \
    keywordFilter.ini \
    strange.txt \
    authorFilter.ini \
    test.txt \
    tidpid_setting.ini
