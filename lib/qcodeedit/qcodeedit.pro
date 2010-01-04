TEMPLATE = lib
TARGET = qcodeedit
DESTDIR = ..

DEPENDPATH += . document language widgets qnfa
INCLUDEPATH += . document language widgets qnfa

CONFIG += staticlib qnfa

QT += xml

# Input
include(lib.pri)
