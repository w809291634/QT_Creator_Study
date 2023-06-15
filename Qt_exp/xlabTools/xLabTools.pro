#-------------------------------------------------
#
# Project created by QtCreator 2018-01-03T10:07:51
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = xLabTools
TEMPLATE = app

# The following define makes your compiler emit warnings if you use
# any feature of Qt which as been marked as deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if you use deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0
CONFIG += c++11

SOURCES += \
        main.cpp \
        mainwindow.cpp \
    qrcode/bitstream.c \
    qrcode/mask.c \
    qrcode/mmask.c \
    qrcode/mqrspec.c \
    qrcode/qrencode.c \
    qrcode/qrinput.c \
    qrcode/qrspec.c \
    qrcode/rsecc.c \
    qrcode/split.c \
    code.cpp \
    updata/updata.cpp \
    lorawanchm.cpp

HEADERS += \
        mainwindow.h \
    qrcode/bitstream.h \
    qrcode/config.h \
    qrcode/mask.h \
    qrcode/mmask.h \
    qrcode/mqrspec.h \
    qrcode/qrencode.h \
    qrcode/qrencode_inner.h \
    qrcode/qrinput.h \
    qrcode/qrspec.h \
    qrcode/rsecc.h \
    qrcode/split.h \
    code.h \
    updata/updata.h \
    lorawanchm.h

FORMS += \
        mainwindow.ui \
    code.ui \
    updata/updata.ui \
    lorawanchm.ui

DEFINES +=HAVE_CONFIG_H

QT += serialport \
        network

RESOURCES += \
    image.qrc

RC_ICONS = image/xLabTools.ico




