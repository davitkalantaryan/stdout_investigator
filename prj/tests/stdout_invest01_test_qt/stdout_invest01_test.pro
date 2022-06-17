#
# File crash_investigateor.pro
# File created : 18 Nov 2021
# Created by : Davit Kalantaryan (davit.kalantaryan@desy.de)
# This file can be used to produce Makefile for daqadcreceiver application
# for PITZ
# CONFIG += 2test

#QMAKE_CXXFLAGS += "-include types.h"
#DEFINES += u_int=unsigned

#DEFINES += DEBUG_APP

DEFINES += STDOUT_INVEST_USE_HOOK01


win32{
	QMAKE_CFLAGS += /Wall /WX
} else {
	QMAKE_CFLAGS += -Wall
	QMAKE_CFLAGS += -Werror
	QMAKE_CFLAGS += -Wno-attributes
        GCCPATH = $$system(which g++)
        message("!!!!!!!!!!! GPPPATH=$$GCCPATH")
}

include("$${PWD}/../../common/common_qt/sys_common.pri")
DESTDIR = $${artifactRoot}/$${SYSTEM_PATH}/test

QT -= core
QT -= gui
CONFIG -= qt


INCLUDEPATH += $${PWD}/../../../include
INCLUDEPATH += $${PWD}/../../../contrib/cpputils/include

SOURCES += "$${PWD}/../../../src/tests/other/main_stdout_invest01_test.cpp"
SOURCES += "$${PWD}/../../../src/core/stdout_analyzer_hook01.c"

HEADERS += \
	"$${PWD}/../../../src/core/stack_investigator_private_internal.h"	\
	$$files($${PWD}/../../../include/*.h,true)

OTHER_FILES +=	\
	"$${PWD}/../../../src/core/analyze/crash_investigator_analyze_leak_only_new_delete.cpp"		\
	"$${PWD}/../../../contrib/cpputils/src/core/cpputils_hashtbl.cpp"				\
	"$${PWD}/../../../src/core/analyze/crash_investigator_analyze_leak_only_new_delete_std_hash.cpp"
