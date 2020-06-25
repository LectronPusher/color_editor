#pragma once

#include "image/image_view.hpp"
#include "tools/tool_panel.hpp"
#include <QWidget>

namespace editor {

class main_window : public QWidget {
	Q_OBJECT
	
public:
	main_window(QWidget *parent = nullptr);
	
 	image::image_view *view;
	tools::tool_panel *color_pan;
	tools::tool_panel *select_pan;
	
}; // window

} // editor
