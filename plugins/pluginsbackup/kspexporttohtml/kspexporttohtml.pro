SOURCES += kspexporttohtml.cpp

HEADERS += kspexporttohtml.h


CONFIG += plugin
TARGET = ../kspexporttohtml

INCLUDEPATH += ../../src

TARGETDEPS += ../../src/../bin/kollegstufe

TEMPLATE = subdirs

