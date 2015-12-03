#-------------------------------------------------
#
# Project created by QtCreator 2015-11-21T13:17:54
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = driving
TEMPLATE = app


SOURCES += main.cpp\
        MainWindow.cpp \
    RoadGraphicScene.cpp \
    GraphicsItems.cpp \
    CarMovement.cpp \
    TelemetryAnalysis.cpp \
    CarTelemetry.cpp

HEADERS  += MainWindow.h \
    RoadGraphicScene.h \
    GraphicsItems.h \
    CarMovement.h \
    TelemetryAnalysis.h \
    CarTelemetry.h

RESOURCES += \
    resources.qrc

TRANSLATIONS += translate.ts
