#-------------------------------------------------
#
# Project created by QtCreator 2014-05-23T16:03:30
#
#-------------------------------------------------

QT       += core
DESTDIR         = $$PWD
QT       -= gui
CONFIG += qt

TARGET = test_generator
CONFIG   += console
CONFIG   -= app_bundle debug_and_release

## EDFfile
CONFIG += largefile c++11

*msvc* {
  CONFIG(debug, debug|release) {
    EDF_LIB_NAME = libEDFlibd.lib
  } else {
    EDF_LIB_NAME = libEDFlib.lib
  }
}
*g++* {
  CONFIG(debug, debug|release) {
    EDF_LIB_NAME = libEDFlibd.a
  } else {
    EDF_LIB_NAME = libEDFlib.a
  }
}

LIBS    += $${PWD}/../../build/$${EDF_LIB_NAME}

message ($${LIBS})
INCLUDEPATH += ../../src


TEMPLATE = app

INCLUDEPATH += ../../src
SOURCES += \
    main.cpp

HEADERS +=
