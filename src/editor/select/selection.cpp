#include "selection.hpp"

namespace editor {
namespace select {

void selection::set_next(selection::select_type next_type) {
	next_selection_type = next_type;
}

void selection::add(const QRegion &region) {
	switch (next_selection_type) {
	case select:
		selected_region |= region;
		break;
	case exclude:
		excluded_region |= region;
	}
	combined_region = selected_region - excluded_region;
}

QRegion selection::selected() const {
	return combined_region;
}

void selection::clear() {
	selected_region = QRegion();
	excluded_region = QRegion();
	combined_region = QRegion();
}

} // select
} // editor
