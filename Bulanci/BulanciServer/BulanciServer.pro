QT -= gui

QT += network widgets

CONFIG += c++11 console
CONFIG -= app_bundle

DEFINES += QT_DEPRECATED_WARNINGS

SOURCES += \
        main.cpp \
    server.cpp \
    serverplayer.cpp

HEADERS += \
    server.h \
    defines.h \
    serverplayer.h
