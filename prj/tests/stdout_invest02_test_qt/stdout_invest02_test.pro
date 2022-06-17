#
# File stdout_invest02_test.pro
# File created : 17 Jun 2022
# Created by : Davit Kalantaryan (davit.kalantaryan@desy.de)
# This file can be used to produce Makefile 
#

#DEFINES += DEBUG_APP

DEFINES += STDOUT_INVEST_USE_HOOK02
DEFINES += STDOUT_INVEST_USING_STATIC_LIB_OR_OBJECTS


win32{
	QMAKE_CFLAGS += /Wall /WX
} else {
	LIBS += -ldl
	QMAKE_CFLAGS += -Wall
	QMAKE_CFLAGS += -Werror
	QMAKE_CFLAGS += -Wno-attributes
        GCCPATH = $$system(which g++)
        message("!!!!!!!!!!! GPPPATH=$$GCCPATH")
}

include("$${PWD}/../../common/common_qt/sys_common.pri")
MAKEFILE = Makefile
DESTDIR = $${artifactRoot}/$${SYSTEM_PATH}/test

QT -= core
QT -= gui
CONFIG -= qt


INCLUDEPATH += $${PWD}/../../../include
INCLUDEPATH += $${PWD}/../../../contrib/cpputils/include

SOURCES += "$${PWD}/../../../src/tests/other/main_stdout_invest02_test.cpp"
SOURCES += "$${PWD}/../../../src/core/stdout_analyzer_hook02_unix.c"

HEADERS += \
	$$files($${PWD}/../../../include/*.h,true)

OTHER_FILES +=	\
	"$${PWD}/../../../contrib/cpputils/src/core/cpputils_hashtbl.cpp"				
