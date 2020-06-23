#include "./toolpanel.hpp"

editor::tools::tool_panel::tool_panel(QWidget *parent, tool_type type) 
: QWidget(parent) {
	if (type == editor::tools::select)
		return;
}
