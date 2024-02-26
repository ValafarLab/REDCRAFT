#-------------------------------------------------
#
# Project created by QtCreator 2017-03-27T13:12:11
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = BackBoneGUIWidget
TEMPLATE = app


SOURCES +=\
        gui.cpp \
    FloatArray.cpp \
    Matrix.cpp \
    Vector.cpp \
    PDBParser.cpp \
    main.cpp \

HEADERS  += gui.h \
    FloatArray.h \
    Matrix.h \
    Vector.h \
    PDBParser.h \


FORMS    += gui.ui

DISTFILES +=

