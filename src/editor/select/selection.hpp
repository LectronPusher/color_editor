#pragma once

#include <QRegion>

namespace editor {
namespace select {

enum select_type {select, exclude};

class selection {
public:
	void add(QRegion points);
	QRegion selected();
	void clear();
	select_type next_selection_type = select;
	
private:
	QRegion selected_region;
	QRegion excluded_region;
	
}; // selection

} // select
} // editor
