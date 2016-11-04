#-------------------------------------------------
#
# Project created by QtCreator 2013-11-16T11:35:37
#
#-------------------------------------------------

QT       += widgets testlib
QT       += core serialport
DESTDIR         = ./

## good run arguments:
## --report_level=short --log_level=error --auto_start_dbg=yes --detect_memory_leak=0

BASE_DIR= $$PWD

DESTDIR=$$BASE_DIR/$$DESTDIR

message(BASE_DIR is [[ $$BASE_DIR ]])
message(PWD is [[ $$PWD ]])

BUILDDIR = $$DESTDIR
UI_DIR = $$BUILDDIR/ui
RCC_DIR = $$BUILDDIR/rcc
MOC_DIR = $$BUILDDIR/moc
OBJECTS_DIR = $$BUILDDIR/obj


## Boost
# define boost directories in user defined local file
# boost_path_variables.pri
#
# BOOST_INCLUDE_DIR = C:\boost_1_57_0
# BOOST_LIB_DIR = C:\boost_1_57_0\lib64-msvc-12.0
include(boost_path_variables.pri)

linux* {
        LIBS += -lboost_math_c99 -lboost_system
}
win* {
    CONFIG +=c++11
    INCLUDEPATH += $${BOOST_INCLUDE_DIR}
    LIBS += -L$${BOOST_LIB_DIR}
    CONFIG(debug, debug|release) {
      LIBS += -llibboost_math_c99-vc120-mt-gd-1_57 \
            -llibboost_container-vc120-mt-gd-1_57
    } else {
      LIBS += -llibboost_math_c99-vc120-mt-1_57 \
            -llibboost_container-vc120-mt-1_57
    }
    DEFINES +=BOOST_TT_HAS_OPERATOR_HPP_INCLUDED
}
DEFINES +=BOOST_TT_HAS_OPERATOR_HPP_INCLUDED
#In order to prevent QT5 migration and Boost: if.hpp: Macro argument mismatch bug
# ensure QMAKE_MOC contains the moc executable path
load(moc)
QMAKE_MOC += -DBOOST_MPL_IF_HPP_INCLUDED
QMAKE_MOC += -DBOOST_MPL_NOT_HPP_INCLUDED
QMAKE_MOC += -DBOOST_CONCEPT_CHECKS_HPP
QMAKE_MOC += -DBOOST_MPL_IDENTITY_HPP_INCLUDED
QMAKE_MOC += -DBOOST_MPL_ALWAYS_HPP_INCLUDED
QMAKE_MOC += -DBOOST_MPL_NEXT_PRIOR_HPP_INCLUDED
QMAKE_MOC += -DBOOST_MPL_PROTECT_HPP_INCLUDED
QMAKE_MOC += -DBOOST_MPL_VOID_HPP_INCLUDED
QMAKE_MOC += -DBOOST_MPL_ABS_HPP_INCLUDED
QMAKE_MOC += -DBOOST_MPL_SIGN_HPP_INCLUDED
QMAKE_MOC += -DBOOST_MPL_GCD_HPP_INCLUDED
QMAKE_MOC += -DBOOST_MPL_LCM_HPP_INCLUDED
QMAKE_MOC += -DBOOST_MPL_EVAL_IF_HPP_INCLUDED
QMAKE_MOC += -DBOOST_MPL_SEQUENCE_TAG_HPP_INCLUDED
QMAKE_MOC += -DBOOST_MPL_FIND_HPP_INCLUDED
QMAKE_MOC += -DBOOST_MPL_CONTAINS_HPP_INCLUDED
QMAKE_MOC += -DBOOST_MPL_O1_SIZE_HPP_INCLUDED
QMAKE_MOC += -DBOOST_MPL_FOLD_HPP_INCLUDED
QMAKE_MOC += -DBOOST_MPL_DEREF_HPP_INCLUDED
QMAKE_MOC += -DBOOST_MPL_BEGIN_END_HPP_INCLUDED
QMAKE_MOC += -DBOOST_MPL_PAIR_HPP_INCLUDED
QMAKE_MOC += -DBOOST_MPL_ITER_FOLD_IF_HPP_INCLUDED
QMAKE_MOC += -DBOOST_MPL_FIND_IF_HPP_INCLUDED
QMAKE_MOC += -DBOOST_MPL_PUSH_BACK_HPP_INCLUDED
QMAKE_MOC += -DBOOST_MPL_PUSH_FRONT_HPP_INCLUDED
QMAKE_MOC += -DBOOST_MPL_REMOVE_IF_HPP_INCLUDED
QMAKE_MOC += -DBOOST_MPL_NEGATE_HPP_INCLUDED
QMAKE_MOC += -DBOOST_MPL_ADVANCE_HPP_INCLUDED
QMAKE_MOC += -DBOOST_MPL_AT_HPP_INCLUDED
QMAKE_MOC += -DBOOST_MPL_ITERATOR_RANGE_HPP_INCLUDED
QMAKE_MOC += -DBOOST_MPL_SIZE_HPP_INCLUDED
QMAKE_MOC += -DBOOST_ITERATOR_CONCEPTS_HPP
QMAKE_MOC += -DBOOST_MPL_ADVANCE_HPP_INCLUDED
QMAKE_MOC += -DBOOST_MPL_AT_HPP_INCLUDED
QMAKE_MOC += -DBOOST_MPL_MULTIPLIES_HPP_INCLUDED
QMAKE_MOC += -DBOOST_TT_TYPE_WITH_ALIGNMENT_INCLUDED

TARGET = EDFFileTests

#CONFIG   -= app_bundle
CONFIG   += console C++11
CONFIG   -= app_bundle
DEFINES += UNIT_TEST

## EDFfile
CONFIG += largefile

INCLUDEPATH += ../../src
win* {
  CONFIG(debug, debug|release) {
    EDF_LIB_NAME = EDFlibd.lib
  } else {
    EDF_LIB_NAME = EDFlib.lib
  }
}
linux* {
  CONFIG(debug, debug|release) {
    EDF_LIB_NAME = EDFlibd.a
  } else {
    EDF_LIB_NAME = EDFlib.a
  }
}



LIBS    += $${PWD}/../../build/$${EDF_LIB_NAME}

#    win32:CONFIG(release, debug|release): LIBS += -L$$PWD -lftd2xx
#    else:win32:CONFIG(debug, debug|release): LIBS += -L$$PWD/ -lftd2xx

SOURCES += \
    #main.cpp \
    #testcoverageobject.cpp \
    tst_edfdate.cpp \
    tst_edffile.cpp \
    tst_edffileheader.cpp \
    tst_edfmeasurementsetup.cpp \
    tst_edfpatient.cpp \
    tst_edfsignaldata.cpp \
    tst_edfsignalheader.cpp\
    tst_edftime.cpp \
    tst_edfannotation.cpp


FORMS +=

HEADERS += \
    #TestRunner.h \
    #testcoverageobject.h \
    #tst_edfdate.h \
    #tst_edffile.h \
    #tst_edffileheader.h \
    #tst_edfmeasurementsetup.h \
    #tst_edfpatient.h \
    #tst_edfsignaldata.h \
    #tst_edfsignalheader.h\
    #tst_edftime.h \
    #tst_edfannotation.h



