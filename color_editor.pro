TEMPLATE = app

QT += widgets

INCLUDEPATH += src/

SOURCES += src/main.cpp \
		   src/editor/mainwindow.cpp \
		   src/editor/image/imagepanel.cpp \
		   src/editor/image/imagearea.cpp \
		   src/editor/tools/toolpanel.cpp

HEADERS += src/editor/mainwindow.hpp \
		   src/editor/image/imagepanel.hpp \
		   src/editor/image/imagearea.hpp \
		   src/editor/tools/toolpanel.hpp

TARGET = color_editor.exe
DESTDIR = build
MOC_DIR = build/tmp
OBJECTS_DIR = build/tmp
