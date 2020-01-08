QT       += core gui network

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++11 console
TARGET = BulanciClient
TEMPLATE = app

DEFINES += QT_DEPRECATED_WARNINGS

SOURCES += \
        main.cpp \
    client.cpp \
    player.cpp \
    renderer.cpp \
    bullet.cpp

HEADERS += \
    client.h \
    defines.h \
    player.h \
    renderer.h \
    direction.h \
    bullet.h
