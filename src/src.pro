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
 kscathegoryeditorwidget.cpp \
 kssubjectinformationwidget.cpp \
 kssubjectstatusbar.cpp \
 xmlloader.cpp \
 ksplugin.cpp \
 ksplugininformation.cpp \
 kspexporttohtml.cpp \
 kspluginengine.cpp \
 kspluginitemwidget.cpp \
 kspluginmanager.cpp \
 kspluginconfigurationdialog.cpp \
 ksconfigcontainer.cpp \
 ksconfigoption.cpp \
 configdialog.cpp \
 ksconfigoptionwidget.cpp \
 xmlencoder.cpp \
 colorbutton.cpp \
 selectablelabel.cpp \
 kspwidgettester.cpp \
 ksiconcatcher.cpp \
 ksstatisticsconfigwidget.cpp
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
 kscathegoryeditorwidget.h \
 kssubjectinformationwidget.h \
 kssubjectstatusbar.h \
 xmlloader.h \
 ksplugin.h \
 ksplugininformation.h \
 kspexporttohtml.h \
 kspluginengine.h \
 kspluginitemwidget.h \
 qclickablelabel.h \
 kspluginmanager.h \
 kspluginconfigurationdialog.h \
 nokeyscrollarea.h \
 ksconfigcontainer.h \
 ksconfigoption.h \
 configdialog.h \
 ksconfigoptionwidget.h \
 xmlencoder.h \
 colorbutton.h \
 selectablelabel.h \
 kspwidgettester.h \
 ksiconcatcher.h \
 ksstatisticsconfigwidget.h
DISTFILES += ../doc/help.htm

TRANSLATIONS += ../translations/german.ts \
../translations/english.ts
