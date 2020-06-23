#pragma once

#include "image/imagepanel.hpp"
#include "tools/toolpanel.hpp"
#include <QWidget>

namespace editor {

class main_window : public QWidget {
	Q_OBJECT
	
public:
	main_window();
	
 	image::image_panel *image_pan;
	tools::tool_panel *color_pan;
	tools::tool_panel *select_pan;
	
}; // window

} // editor
