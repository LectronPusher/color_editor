TEMPLATE = app

QT = core widgets gui

INCLUDEPATH = src/

SOURCES = src/main.cpp \
		  src/editor/mainwindow.cpp \
		  src/editor/image/imagepanel.cpp \
		  src/editor/tools/toolpanel.cpp

HEADERS = src/editor/mainwindow.cpp \
		  src/editor/image/imagepanel.cpp \
		  src/editor/tools/toolpanel.cpp

TARGET = color_editor.exe
DESTDIR = build
MOC_DIR = build/tmp
OBJECTS_DIR = build/tmp
