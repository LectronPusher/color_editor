#include "selection.hpp"

namespace editor {
namespace select {

void selection::add(const select_region &region) {
	switch (region.second) {
		case select:
			selected_region |= region.first;
			break;
		case exclude:
			excluded_region |= region.first;
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
