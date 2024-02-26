#-------------------------------------------------
#
# Project created by QtCreator 2019-01-30T13:10:46
#
#-------------------------------------------------

QT       += core gui charts \
    widgets

greaterThan(QT_MAJOR_VERSION, 5): QT += widgets

TARGET = redcraft
RESOURCES = breeze.qrc \
    icons.qrc
TEMPLATE = app

# The following define makes your compiler emit warnings if you use
# any feature of Qt which has been marked as deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if you use deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

CONFIG += c++11

SOURCES += \
    dynamicprofile.cpp \
    histogram.cpp \
        main.cpp \
        mainwindow.cpp \
    analysissuite.cpp \
    redcat.cpp \
    scatter.cpp \
    dialogsearchdepth.cpp \
    stage0.cpp \
    stage1.cpp \
    stage2.cpp \
    io.cpp \
    config.cpp

HEADERS += \
    dynamicprofile.h \
    histogram.h \
        mainwindow.h \
    analysissuite.h \
    scatter.h \
    dialogsearchdepth.h

FORMS += \
        mainwindow.ui \
    analysissuite.ui \
    dialogsearchdepth.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target
