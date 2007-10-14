SOURCES += main.cpp \
 kollegstufeparent.cpp \
 ksdebugoutput.cpp \
 xmlparser.cpp \
 error.cpp \
 dateConverter.cpp \
 examitem.cpp \
 ksplattformspec.cpp \
 ksexamproperties.cpp \
 ksabout.cpp \
 kssubjectproperties.cpp \
 ksdatabaseselection.cpp \
 ksdatabaseproperties.cpp \
 ksstatisticsdialog.cpp \
 ksstatisticswidget.cpp \
 ksstatisticsitem.cpp \
 ksconfigure.cpp \
 kscathegoryeditorwidget.cpp
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
 ksexamproperties.h \
 ksabout.h \
 kssubjectproperties.h \
 ksdatabaseselection.h \
 ksdatabaseproperties.h \
 ksstatisticsdialog.h \
 ksstatisticswidget.h \
 ksstatisticsitem.h \
 ksconfigure.h \
 kscathegoryeditorwidget.h
