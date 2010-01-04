include( ../settings.pri )
TARGET = ../coquille
TEMPLATE = app

# Input
HEADERS += coqtop.h \
    mainwindow.h \
    textstd.h \
    texteditor.h \
    tabmanager.h \
    documentsendcommand.h \
    settingsdialog.h \
    debugdialog.h \
    filelink.h \
    settings.h \
    documentationwidget.h
FORMS += mainwindow.ui \
    textstd.ui \
    tabmanager.ui \
    settingsdialog.ui \
    debugdialog.ui \
    documentationwidget.ui
SOURCES += coqtop.cpp \
    main.cpp \
    mainwindow.cpp \
    textstd.cpp \
    texteditor.cpp \
    tabmanager.cpp \
    documentsendcommand.cpp \
    settingsdialog.cpp \
    debugdialog.cpp \
    filelink.cpp \
    settings.cpp \
    documentationwidget.cpp
INCLUDEPATH += ../lib/qcodeedit \
    ../lib/qcodeedit/document \
    ../lib/qcodeedit/snippets \
    ../lib/qcodeedit/widgets \
    ../lib/qticonloader
terminal:INCLUDEPATH += ../lib/qtermwidget
LIBS += ../lib/libqcodeedit.a \
    ../lib/libqticonloader.a
terminal:LIBS += ../lib/libqtermwidget.a
DEPENDPATH += ../lib/libqcodeedit.a \
    ../lib/libqticonloader.a
terminal:DEPENDPATH += ../lib/libqtermwidget.a
QT += xml
RESOURCES += resources/syntax.qrc \
    resources/images.qrc \
    resources/symbols.qrc \
    resources/documentation.qrc \
    resources/languages.qrc
TRANSLATIONS = resources/languages/coquille_fr.ts \
    resources/languages/coquille_us.ts \
    resources/languages/coquille_es.ts \
    resources/languages/coquille_ro.ts
terminal:DEFINES += TERMINAL
