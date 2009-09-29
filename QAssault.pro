# -------------------------------------------------
# Project created by QtCreator 2009-08-18T22:06:27
# -------------------------------------------------
include(C:\GiulianoXT\bin\qtstatemachine-1.1-opensource\src\statemachine.pri)

# include(C:\GiulianoXT\bin\qtanimationframework-2.3-opensource\src\qtanimationframework.pri)
TARGET = QAssault
TEMPLATE = app
SOURCES += main.cpp \
    assaultmainwindow.cpp \
    assaultscene.cpp \
    piece.cpp \
    assaultitems.cpp \
    util.cpp \
    player.cpp \
    game.cpp \
    transitions.cpp \
    scoreboard.cpp \
    aiplayer.cpp \
    globals.cpp \
    animations.cpp \
    minimax.cpp
HEADERS += assaultmainwindow.h \
    assaultscene.h \
    globals.h \
    piece.h \
    assaultitems.h \
    util.h \
    player.h \
    game.h \
    transitions.h \
    scoreboard.h \
    aiplayer.h \
    animations.h \
    minimax.h
FORMS += assaultmainwindow.ui
CONFIG += console
RESOURCES += images.qrc
QMAKE_RESOURCE_FLAGS += -no-compress
DEFINES += QT_STATEMACHINE_SOLUTION \
    QT_NO_ANIMATION
INCLUDEPATH += C:/GiulianoXT/bin/qtstatemachine-1.1-opensource/src
