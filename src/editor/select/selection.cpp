#include "selection.hpp"

namespace editor {
namespace select {

void selection::add(QRegion region) {
	if (next_selection_type == select)
		selected_region |= region;
	else
		excluded_region |= region;
}

QRegion selection::selected() {
	if (!excluded_region.isEmpty())
		return selected_region - excluded_region;
	return selected_region;
}

void selection::clear() {
	selected_region = QRegion();
	excluded_region = QRegion();
}

} // select
} // editor
