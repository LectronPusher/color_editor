#include "selection.hpp"

namespace editor {
namespace select {

void selection::add_selected(point_set points) {
	point_set &set = (next_selection_type == select) ? selected : excluded;
	set |= points;
}

point_set selection::selected_points() {
	point_set points = selected;
	if (!excluded.is_empty())
		points -= excluded;
	return points;
}

void selection::clear() {
	selected -= selected;
	excluded -= excluded;
}

} // select
} // editor
