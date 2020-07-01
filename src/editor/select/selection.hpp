#pragma once

#include "../point_set.hpp"

namespace editor {
namespace select {

enum select_type {select, exclude};

class selection {
public:
	void add_selected(point_set points);
	point_set selected_points();
	void clear();
	select_type next_selection_type = select;
	
private:
	point_set selected;
	point_set excluded;
	
}; // selection

} // select
} // editor
