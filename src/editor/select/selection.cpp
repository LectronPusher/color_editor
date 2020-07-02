#include "selection.hpp"

namespace editor {
namespace select {

void selection::add_selected(QRegion region) {
	QRegion &set = (next_selection_type == select) ? selected : excluded;
	set |= region;
}

QRegion selection::selected_region() {
	QRegion region = selected;
	if (!excluded.isEmpty())
		region -= excluded;
	return region;
}

void selection::clear() {
	selected -= selected;
	excluded -= excluded;
}

} // select
} // editor
