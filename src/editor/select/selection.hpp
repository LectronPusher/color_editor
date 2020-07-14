#pragma once

#include <QRegion>
#include <QPair>

namespace editor {
namespace select {

class selection {
public:
	enum select_type {select, exclude};
	typedef QPair<QRegion, select_type> select_region;
	
	void add(const select_region &region);
	void clear();
	QRegion selected() const;
	
private:
	QRegion selected_region;
	QRegion excluded_region;
	QRegion combined_region;
	
}; // selection

} // select
} // editor
