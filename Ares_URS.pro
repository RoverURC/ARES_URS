#-------------------------------------------------
#
# Project created by QtCreator 2014-03-08T17:11:41
#
#-------------------------------------------------

QT       += core gui network

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = Ares_URS
TEMPLATE = app

SOURCES += main.cpp\
        mainwindow.cpp \
    modbusclient.cpp \
    modbusserver.cpp \
    connectionstatuswidget.cpp \
    settingswindow.cpp \
    qjoystick.cpp \
    joystickview.cpp \
    rover.cpp \
    manipulator.cpp

HEADERS  += mainwindow.h \
    modbusclient.h \
    modbusserver.h \
    modbus.h \
    connectionstatuswidget.h \
    settingswindow.h \
    qjoystick.h \
    joystickview.h \
    rover.h \
    manipulator.h \
    motorcodes.h

FORMS    += mainwindow.ui \
    settingswindow.ui

LIBS += -lSDL
