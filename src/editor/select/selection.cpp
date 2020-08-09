#include "selection.hpp"

using namespace editor::select;

QRegion selection::selected_region() const {
	return combined;
}

QRect selection::region_rect() const {
	return combined.boundingRect();
}

bool selection::has_selection() const {
	return !applied_changes.empty();
}

void selection::add_region(const QRegion &region, select_type s_type) {
	switch (s_type) {
		case select:
			apply_change({s_type, region - selected});
			break;
		case exclude:
			apply_change({s_type, region - excluded});
			break;
		case remove:
			apply_change({s_type, {region & selected, region & excluded}});
			break;
		case clear:
			apply_change({s_type, {selected, excluded}});
	}
	undone_changes.clear();
}

void selection::clear_regions() {
	add_region(QRegion(), clear);
}

void selection::apply_change(const region_change &data) {
	switch (data.s_type) {
		case select:
			selected |= data.added_region;
			break;
		case exclude:
			excluded |= data.added_region;
			break;
		case remove:
			if (applied_changes.empty())
				return;
			selected -= data.removed_regions.selected;
			excluded -= data.removed_regions.excluded;
			break;
		case clear:
			if (applied_changes.empty())
				return;
			else if (applied_changes.front().s_type == clear)
				return;
			selected = QRegion();
			excluded = QRegion();
	}
	applied_changes.push_front(data);
	update_combined();
}

void selection::update_combined() {
	QRegion old = combined;
	combined = selected - excluded;
	QRect difference = (combined ^ old).boundingRect();
	if (!difference.isEmpty())
		emit contents_updated(difference);
	if (old.boundingRect() != combined.boundingRect())
		emit region_boundary_updated(combined.boundingRect());
}

void selection::undo() {
	if (!applied_changes.empty()) {
		const region_change &data = applied_changes.front();
		switch (data.s_type) {
			case select:
				selected -= data.added_region;
				break;
			case exclude:
				excluded -= data.added_region;
				break;
			case remove:
				selected |= data.removed_regions.selected;
				excluded |= data.removed_regions.excluded;
				break;
			case clear:
				selected = data.removed_regions.selected;
				excluded = data.removed_regions.excluded;
		}
		undone_changes.push_front(data);
		applied_changes.pop_front();
		update_combined();
	}
}

void selection::redo() {
	if (!undone_changes.empty()) {
		apply_change(undone_changes.front());
		undone_changes.pop_front();
	}
}

void selection::combine_recent_changes(int n) {
	region_change parent = applied_changes.front();
	applied_changes.pop_front();
	for (int i = 1; i < n; ++i) {
		const region_change &child = applied_changes.front();
		if (parent.s_type == remove || parent.s_type == clear) {
			parent.removed_regions.selected |= child.removed_regions.selected;
			parent.removed_regions.excluded |= child.removed_regions.excluded;
		} else {
			parent.added_region |= child.added_region;
		}
		applied_changes.pop_front();
	}
	applied_changes.push_front(parent);
}

// region_change
selection::region_change::region_change(select_type type, QRegion region)
: s_type(type), added_region(region) {}

selection::region_change::region_change(select_type type, region_pair regions)
: s_type(type), removed_regions(regions) {}

selection::region_change::region_change(const region_change &other) {
	s_type = other.s_type;
	if (s_type == remove || s_type == clear)
		new(&removed_regions) region_pair(other.removed_regions);
	else
		new(&added_region) QRegion(other.added_region);
}

selection::region_change selection::region_change::operator=(const region_change &other) {
	s_type = other.s_type;
	if (s_type == remove || s_type == clear)
		new(&removed_regions) region_pair(other.removed_regions);
	else
		new(&added_region) QRegion(other.added_region);
	return *this;
}

selection::region_change::~region_change() {
	if (s_type == remove || s_type == clear)
		removed_regions.~region_pair();
	else
		added_region.~QRegion();
}
// end region_change
