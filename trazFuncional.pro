#-------------------------------------------------
#
# Project created by QtCreator 2016-09-22T11:20:16
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = trazFuncional
TEMPLATE = app

include(QtXlsxWriter-master/src/xlsx/qtxlsx.pri)

SOURCES += main.cpp\
        mainwindow.cpp \
    codeeditor.cpp \
    analizador.cpp

HEADERS  += mainwindow.h \
    codeeditor.h \
    analizador.h

FORMS    += mainwindow.ui

RESOURCES +=

RC_FILE = app_icon.rc

QMAKE_CXXFLAGS += -std=gnu++11
