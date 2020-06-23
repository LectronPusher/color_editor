TEMPLATE = app

QT += widgets

INCLUDEPATH += src/

SOURCES += src/main.cpp \
		   src/editor/main_window.cpp \
		   src/editor/image/image_panel.cpp \
		   src/editor/image/image_base.cpp \
		   src/editor/tools/tool_panel.cpp

HEADERS += src/editor/main_window.hpp \
		   src/editor/image/image_panel.hpp \
		   src/editor/image/image_base.hpp \
		   src/editor/tools/tool_panel.hpp

TARGET = color_editor.exe
DESTDIR = build
MOC_DIR = build/tmp
OBJECTS_DIR = build/tmp
