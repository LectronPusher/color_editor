#include "selection.hpp"

namespace editor {
namespace select {

void selection::add(const select_region &region_pair) {
	QRegion region = region_pair.first;
	switch (region_pair.second) {
		case select:
			selected_region |= region;
			break;
		case exclude:
			excluded_region |= region;
			break;
		case remove:
			excluded_region -= region;
			selected_region -= region;
			break;
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
