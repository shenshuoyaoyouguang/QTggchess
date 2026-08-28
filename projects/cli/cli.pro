TARGET = cutechess-cli
DESTDIR = $$PWD

include(../lib/lib.pri)
include(../lib/libexport.pri)
# ECO qrc lives in lib/res; see gui.pro for the same note.
include(../lib/res/res.pri)

CUTECHESS_CLI_VERSION = 1.1.0

macx-xcode {
    DEFINES += CUTECHESS_CLI_VERSION=\"$$CUTECHESS_CLI_VERSION\"
} else {
    OBJECTS_DIR = .obj/
    MOC_DIR = .moc/
    RCC_DIR = .rcc/
    DEFINES += CUTECHESS_CLI_VERSION=\\\"$$CUTECHESS_CLI_VERSION\\\"
}

win32 {
    CONFIG += console
}

!win32-msvc* {
	QMAKE_CXXFLAGS += -Wextra
}

mac {
    CONFIG -= app_bundle
}

QT = core sql

# Code
include(src/src.pri)

# Resources
include(res/res.pri)
