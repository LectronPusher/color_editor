#pragma once

#include <QWidget>

namespace editor::tools {

enum tool_type {color, select};

class tool_panel : public QWidget {
	Q_OBJECT
	
public:
	tool_panel(QWidget *parent, tool_type type);
}; // tool_panel

} // editor::image
