#pragma once

#include <QRegion>

namespace editor {
namespace select {

class selection {
public:
	enum select_type {select, exclude};
	
	void set_next(select_type next_type);
	void add(const QRegion &points);
	void clear();
	QRegion selected() const;
	
private:
	select_type next_selection_type;
	QRegion selected_region;
	QRegion excluded_region;
	QRegion combined_region;
	
}; // selection

} // select
} // editor
