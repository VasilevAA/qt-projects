

QT       += core gui \
           concurrent widgets
QMAKE_CXXFLAGS += -std=c++14
CONFIG += c++14 \


greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = Graph_lab
TEMPLATE = app


DEFINES += QT_DEPRECATED_WARNINGS

SOURCES += \
        main.cpp \
        mainwindow.cpp \
    graphwindow.cpp \
    mgraphview.cpp \
    node.cpp \
    edge.cpp \
    mscene.cpp \
    helpbrowser.cpp

HEADERS += \
        mainwindow.h \
    graphwindow.h \
    mgraphview.h \
    node.h \
    edge.h \
    mscene.h \
    helpbrowser.h

FORMS += \
        mainwindow.ui \
    graphwindow.ui

RESOURCES += \
    icons.qrc

DISTFILES +=



