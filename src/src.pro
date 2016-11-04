#-------------------------------------------------
#
# Project created by QtCreator 2013-11-16T11:35:37
#
#-------------------------------------------------


DESTDIR         = $$PWD


TEMPLATE = lib
CONFIG += static
DESTDIR = $${PWD}/../build/
TARGET = EDFlib

*msvc*{
  CONFIG(debug, debug|release) {
    TARGET = libEDFlibd
  } else {
    TARGET = libEDFlib
  }
}
*g++*{
  CONFIG(debug, debug|release) {
    TARGET = EDFlibd
  } else {
    TARGET = EDFlib
  }
}


## QT Modules
QT += core
## EDFfile
CONFIG += largefile
CONFIG += c++14
CONFIG -= debug_and_release

CONFIG(release, debug|release) {
    OUT_DIR=release
    QMAKE_CFLAGS += -O2
}

SOURCES += \
    EDFError.cpp \
    EDFFileManager.cpp \
    EDFFile.cpp \
    EDFPatient.cpp \
    EDFFileHeader.cpp \
    EDFAnnotation.cpp\
    EDFDate.cpp \
    EDFTime.cpp\
    EDFSignalHeader.cpp\
    EDFMeasurementSetup.cpp \
    EDFSignalData.cpp


HEADERS += \
    EDFError.h \
    EDFFileManager.h \
    EDFFile.h \
    EDFPatient.h \
    EDFFileHeader.h \
    EDFAnnotation.h\
    EDFDate.h \
    EDFTime.h\
    EDFSignalHeader.h\
    EDFMeasurementSetup.h \
    EDFSignalData.h \




