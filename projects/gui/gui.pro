macx {
    TARGET = "Cute Chess"
} else {
    TARGET = cutechess
}

DESTDIR = $$PWD

include(../lib/lib.pri)
include(../lib/libexport.pri)

CUTECHESS_VERSION = 1.1.0

macx-xcode {
    DEFINES += CUTECHESS_VERSION=\"$$CUTECHESS_VERSION\"
}else {
    OBJECTS_DIR = .obj/
    MOC_DIR = .moc/
    RCC_DIR = .rcc/
    DEFINES += CUTECHESS_VERSION=\\\"$$CUTECHESS_VERSION\\\"
}

QT += svg widgets concurrent printsupport sql

win32 {
    CONFIG(debug, debug|release) {
	CONFIG += console
    }
    RC_FILE = res/icons/cutechess_win.rc
}

!win32-msvc* {
	QMAKE_CXXFLAGS += -Wextra
}

macx {
    ICON = res/icons/cutechess_mac.icns
}

UI_HEADERS_DIR = src

include(src/src.pri)
include(ui/ui.pri)
include(res/res.pri)
include(3rdparty/qcustomplot/qcustomplot.pri)

CONFIG(debug, debug|release) {
    include(3rdparty/modeltest/modeltest.pri)
}

# Optional OpenCV-based screen capture feature. On by default (matches the MSVC build).
# Disable with CONFIG+=no_capture when OpenCV is unavailable (e.g. MinGW without OpenCV).
!contains(CONFIG, no_capture) {
    DEFINES += QTGG_CAPTURE
    INCLUDEPATH += $$(OPENCV_INC)
    LIBS += -L$$(OPENCV_LIB) -lopencv_world
    SOURCES += $$PWD/src/capture.cpp
}
