QT       += core gui network

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = simplemqttclient
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

SOURCES += main.cpp\
        mainwindow.cpp

HEADERS  += mainwindow.h
FORMS    += mainwindow.ui

# window build release
win32:CONFIG(release, debug|release):{
    mingw:{
        LIBS += -L$$PWD/lib/win64/mingw -lQt5Mqtt
        message("mingw release")
    }
    msvc:{
        LIBS += -L$$PWD/lib/win64/msvc -lQt5Mqtt
        message("msvc release")
    }
}
# window build debug
else:win32:CONFIG(debug, debug|release): {
    mingw:{
        LIBS += -L$$PWD/lib/win64/mingw -lQt5Mqttd
        message("mingw debug")
    }
    msvc:{
        LIBS += -L$$PWD/lib/win64/msvc -lQt5Mqttd
        message("msvc debug")
    }
}
# unix build
else:unix: {
    contains(QT_ARCH, arm64){
        unix: LIBS += -L$$PWD/lib/linux64/arm64/ -lQt5Mqtt
        message("unix arm64")
    }else{
        unix:!macx: LIBS += -L$$PWD/lib/linux64/x86/ -lQt5Mqtt
        message("unix x86")
    }
}

INCLUDEPATH += $$PWD/include
DEPENDPATH += $$PWD/include

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target
