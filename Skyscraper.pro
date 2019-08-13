#-------------------------------------------------
#
# Project created by QtCreator 2019-08-07T16:40:15
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = Skyscraper
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
        src/centralwidget.cpp \
        src/gridcell.cpp \
        src/gridcellid.cpp \
        src/gridcomponent.cpp \
        src/gridsolver.cpp \
        src/main.cpp \
        src/mainwindow.cpp

HEADERS += \
        src/centralwidget.h \
        src/gridcell.h \
        src/gridcellid.h \
        src/gridcomponent.h \
        src/gridsolver.h \
        src/mainwindow.h

FORMS += \
        src/mainwindow.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target
