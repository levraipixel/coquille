include( ../settings.pri )

TEMPLATE = subdirs
SUBDIRS += qcodeedit qticonloader

terminal {
    SUBDIRS += qtermwidget
}
