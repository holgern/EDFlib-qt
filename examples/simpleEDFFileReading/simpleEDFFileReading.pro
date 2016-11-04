#-------------------------------------------------
#
# Project created by QtCreator 2014-05-23T16:03:30
#
#-------------------------------------------------

QT       += core
DESTDIR         = $$PWD
QT       -= gui
CONFIG += qt

TARGET = test_reader
CONFIG   += console c++11
CONFIG   -= app_bundle debug_and_release

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

INCLUDEPATH += ../../src
#BUILDDIR = $$DESTDIR
#UI_DIR = $$BUILDDIR/ui
#RCC_DIR = $$BUILDDIR/rcc
#MOC_DIR = $$BUILDDIR/moc
#BJECTS_DIR = $$BUILDDIR/obj


## EDFfile
CONFIG += largefile

TEMPLATE = app


INCLUDEPATH += ../../src
SOURCES += \
    main.cpp

HEADERS +=
