#-------------------------------------------------
#
# Project created by QtCreator 2016-01-14T16:53:38
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = NIMP

CONFIG += c++11

QMAKE_CXXFLAGS += -std=c++11

SOURCES += src/Main.cpp \
    src/Splitter.cpp \
    src/SplitWidget.cpp \
    src/Expander.cpp

HEADERS  += \
    include/Global.hpp \
    include/Main.hpp \
    include/Splitter.hpp \
    include/SplitWidget.hpp \
    include/Expander.hpp

RESOURCES += \
    images.qrc
