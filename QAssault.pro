# -------------------------------------------------
# Project created by QtCreator 2009-08-18T22:06:27
# -------------------------------------------------
TARGET = QAssault
TEMPLATE = app
SOURCES += main.cpp \
    assaultmainwindow.cpp \
    assaultscene.cpp \
    piece.cpp \
    globals.cpp \
    assaultitems.cpp \
    util.cpp \
    player.cpp
HEADERS += assaultmainwindow.h \
    assaultscene.h \
    globals.h \
    piece.h \
    assaultitems.h \
    util.h \
    player.h
FORMS += assaultmainwindow.ui
CONFIG += console
RESOURCES += images.qrc
QMAKE_RESOURCE_FLAGS += -no-compress
