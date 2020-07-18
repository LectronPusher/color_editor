TEMPLATE = app

QT += widgets

INCLUDEPATH += src/

SOURCES += src/main.cpp \
		   src/editor/main_window.cpp \
		   src/editor/mouse_mode.cpp \
		   src/editor/image/image_view.cpp \
		   src/editor/image/image_base.cpp \
		   src/editor/image/mask.cpp \
		   src/editor/select/selector.cpp \
		   src/editor/select/selection.cpp \
		   src/editor/select/selector_types.cpp \
		   src/editor/color/effect.cpp \
		   src/editor/color/effect_types.cpp \
		   src/editor/color/color_label.cpp

HEADERS += src/editor/main_window.hpp \
		   src/editor/widget_stack.hpp \
		   src/editor/mouse_mode.hpp \
		   src/editor/image/image_view.hpp \
		   src/editor/image/image_base.hpp \
		   src/editor/image/mask.hpp \
		   src/editor/select/selector.hpp \
		   src/editor/select/selection.hpp \
		   src/editor/select/selector_types.hpp \
		   src/editor/color/effect.hpp \
		   src/editor/color/effect_types.hpp \
		   src/editor/color/color_label.hpp

# external
INCLUDEPATH += external/ColorUtils/
SOURCES += external/ColorUtils/ColorUtils.cpp
HEADERS += external/ColorUtils/ColorUtils.hpp

TARGET = color_editor.exe
DESTDIR = build
MOC_DIR = build/tmp
OBJECTS_DIR = build/tmp
