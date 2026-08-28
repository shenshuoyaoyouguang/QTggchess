TEMPLATE = lib
TARGET = cutechess
QT = core sql
DESTDIR = $$PWD

!win32-msvc* {
	QMAKE_CXXFLAGS += -Wextra
}

include(libexport.pri)
include(src/src.pri)
# NOTE: do NOT include res/res.pri here. eco.cpp's Q_INIT_RESOURCE(eco)
# call expands to an extern reference; each consumer (gui, cli) must
# compile its own qrc_eco.cpp so the symbol resolves in the final
# binary. lib itself does not need the resource to be embedded.

OBJECTS_DIR = .obj
MOC_DIR = .moc
RCC_DIR = .rcc
