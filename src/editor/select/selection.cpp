#include "selection.hpp"

namespace editor {
namespace select {

void selection::add(const QRegion &region) {
	if (next_selection_type == select)
		selected_region |= region;
	else
		excluded_region |= region;
}

QRegion selection::selected() const {
	return selected_region - excluded_region;
}

void selection::clear() {
	selected_region = QRegion();
	excluded_region = QRegion();
}

} // select
} // editor
