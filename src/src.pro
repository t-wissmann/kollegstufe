CONFIG += warn_on \
	  thread \
          qt \
 assistant
TARGET = ../bin/kollegstufe

DISTFILES += ../doc/help.htm

TRANSLATIONS += ../translations/german.ts \
../translations/english.ts

SUBDIRS +=TEMPLATE = app

SOURCES += main.cpp \
core/configdialog.cpp \
core/dateConverter.cpp \
core/kollegstufeparent.cpp \
core/ksconfigcontainer.cpp \
core/ksconfigoption.cpp \
core/ksconfigoptionwidget.cpp \
core/ksplattformspec.cpp \
dialogs/ksabout.cpp \
dialogs/ksconfigure.cpp \
dialogs/ksdatabaseproperties.cpp \
dialogs/ksdatabaseselection.cpp \
dialogs/ksexamproperties.cpp \
dialogs/ksstatisticsdialog.cpp \
dialogs/kssubjectproperties.cpp \
io/error.cpp \
io/ksdebugoutput.cpp \
io/ksiconcatcher.cpp \
io/xmlencoder.cpp \
io/xmlloader.cpp \
io/xmlparser.cpp \
pluginengine/kspluginconfigurationdialog.cpp \
pluginengine/ksplugin.cpp \
pluginengine/kspluginengine.cpp \
pluginengine/ksplugininformation.cpp \
pluginengine/kspluginitemwidget.cpp \
pluginengine/kspluginmanager.cpp \
widgets/colorbutton.cpp \
widgets/examitem.cpp \
widgets/kscathegoryeditorwidget.cpp \
widgets/ksstatisticsconfigwidget.cpp \
widgets/ksstatisticsitem.cpp \
widgets/ksstatisticswidget.cpp \
widgets/kssubjectinformationwidget.cpp \
widgets/kssubjectstatusbar.cpp \
widgets/selectablelabel.cpp \
 widgets/ksfilterwidget.cpp \
 dialogs/ksdebugdialog.cpp \
 widgets/kssemesterlisteditor.cpp \
 widgets/kssemesteritemwidget.cpp
HEADERS += core/configdialog.h \
core/dateConverter.h \
core/kollegstufeparent.h \
core/ksconfigcontainer.h \
core/ksconfigoption.h \
core/ksconfigoptionwidget.h \
core/ksplattformspec.h \
dialogs/ksabout.h \
dialogs/ksconfigure.h \
dialogs/ksdatabaseproperties.h \
dialogs/ksdatabaseselection.h \
dialogs/ksexamproperties.h \
dialogs/ksstatisticsdialog.h \
dialogs/kssubjectproperties.h \
io/error.h \
io/ksdebugoutput.h \
io/ksiconcatcher.h \
io/xmlencoder.h \
io/xmlloader.h \
io/xmlparser.h \
pluginengine/kspluginconfigurationdialog.h \
pluginengine/kspluginengine.h \
pluginengine/ksplugin.h \
pluginengine/ksplugininformation.h \
pluginengine/kspluginitemwidget.h \
pluginengine/kspluginmanager.h \
widgets/colorbutton.h \
widgets/examitem.h \
widgets/kscathegoryeditorwidget.h \
widgets/ksstatisticsconfigwidget.h \
widgets/ksstatisticsitem.h \
widgets/ksstatisticswidget.h \
widgets/kssubjectinformationwidget.h \
widgets/kssubjectstatusbar.h \
widgets/nokeyscrollarea.h \
widgets/qclickablelabel.h \
widgets/selectablelabel.h \
 pluginengine/ksplugininterface.h \
 widgets/ksfilterwidget.h \
 dialogs/ksdebugdialog.h \
 widgets/kssemesterlisteditor.h \
 widgets/kssemesteritemwidget.h
