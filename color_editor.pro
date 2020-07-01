TEMPLATE = app

QT += widgets

INCLUDEPATH += src/

SOURCES += src/main.cpp \
		   src/editor/main_window.cpp \
		   src/editor/point_set.cpp \
		   src/editor/image/image_view.cpp \
		   src/editor/image/image_base.cpp \
		   src/editor/select/selector.cpp \
		   src/editor/select/selection.cpp \
		   src/editor/select/selector_types.cpp \

HEADERS += src/editor/main_window.hpp \
		   src/editor/point_set.hpp \
		   src/editor/widget_stack.hpp \
		   src/editor/image/image_view.hpp \
		   src/editor/image/image_base.hpp \
		   src/editor/select/selector.hpp \
		   src/editor/select/selection.hpp \
		   src/editor/select/selector_types.hpp \

TARGET = color_editor.exe
DESTDIR = build
MOC_DIR = build/tmp
OBJECTS_DIR = build/tmp
