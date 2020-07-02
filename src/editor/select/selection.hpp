#pragma once

#include <QRegion>

namespace editor {
namespace select {

enum select_type {select, exclude};

class selection {
public:
	void add_selected(QRegion points);
	QRegion selected_region();
	void clear();
	select_type next_selection_type = select;
	
private:
	QRegion selected;
	QRegion excluded;
	
}; // selection

} // select
} // editor
