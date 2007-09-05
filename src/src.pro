SOURCES += main.cpp \
 kollegstufeparent.cpp \
 ksdebugoutput.cpp \
 xmlparser.cpp \
 error.cpp \
 dateConverter.cpp \
 examitem.cpp \
 ksplattformspec.cpp \
 ksexamproperties.cpp
TEMPLATE = app
CONFIG += warn_on \
	  thread \
          qt \
 assistant
TARGET = ../bin/kollegstufe

HEADERS += kollegstufeparent.h \
ksdebugoutput.h \
 xmlparser.h \
 error.h \
 dateConverter.h \
 examitem.h \
 ksplattformspec.h \
 ksexamproperties.h
