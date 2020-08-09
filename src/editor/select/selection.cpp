#include "selection.hpp"

using namespace editor::select;

selection::selection(QObject *parent) : QObject(parent) {}

QRegion selection::selected_region() const {
	return combined;
}

QRect selection::region_rect() const {
	return combined_boundary;
}

bool selection::has_selection() const {
	return !applied.empty();
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
			if (applied.empty())
				return;
			apply_change({s_type, {region & selected, region & excluded}});
			break;
		case clear:
			if (applied.empty() || (applied.front().s_type == clear))
				return;
			apply_change({s_type, {selected, excluded}});
	}
	undone.clear();
}

void selection::apply_change(const change &change) {
	switch (change.s_type) {
		case select:
			selected |= change.added_region;
			break;
		case exclude:
			excluded |= change.added_region;
			break;
		case remove:
			selected -= change.removed_regions.selected;
			excluded -= change.removed_regions.excluded;
			break;
		case clear:
			selected = QRegion();
			excluded = QRegion();
	}
	applied.push_front(change);
	update_combined();
}

void selection::undo_change(const change &change) {
	switch (change.s_type) {
		case select:
			selected -= change.added_region;
			break;
		case exclude:
			excluded -= change.added_region;
			break;
		case remove:
			selected |= change.removed_regions.selected;
			excluded |= change.removed_regions.excluded;
			break;
		case clear:
			selected = change.removed_regions.selected;
			excluded = change.removed_regions.excluded;
	}
	undone.push_front(change);
	update_combined();
}

void selection::undo() {
	if (!applied.empty()) {
		undo_change(applied.front());
		applied.pop_front();
	}
}

void selection::redo() {
	if (!undone.empty()) {
		apply_change(undone.front());
		undone.pop_front();
	}
}

void selection::update_combined() {
	QRegion old_region = combined;
	combined = selected - excluded;
	QRect difference = (combined ^ old_region).boundingRect();
	if (!difference.isEmpty())
		emit contents_updated(difference);
	
	QRect old_rect = combined_boundary;
	combined_boundary = combined.boundingRect();
	if (old_rect != combined_boundary)
		emit boundary_updated(combined_boundary);
}

void selection::combine_changes(int n) {
	change parent = applied.front();
	applied.pop_front();
	for (int i = 1; i < n; ++i) {
		const change &child = applied.front();
		switch (parent.s_type) {
			case select:
			case exclude:
				parent.added_region |= child.added_region;
				break;
			case remove:
			case clear:
				parent.removed_regions.selected |= child.removed_regions.selected;
				parent.removed_regions.excluded |= child.removed_regions.excluded;
		}
		applied.pop_front();
	}
	applied.push_front(parent);
}

// region_change
selection::change::change(select_type type, QRegion region)
: s_type(type), added_region(region) {}

selection::change::change(select_type type, region_pair regions)
: s_type(type), removed_regions(regions) {}

selection::change::change(const change &other) {
	s_type = other.s_type;
	if (s_type == remove || s_type == clear)
		new(&removed_regions) region_pair(other.removed_regions);
	else
		new(&added_region) QRegion(other.added_region);
}

selection::change selection::change::operator=(const change &other) {
	s_type = other.s_type;
	if (s_type == remove || s_type == clear)
		new(&removed_regions) region_pair(other.removed_regions);
	else
		new(&added_region) QRegion(other.added_region);
	return *this;
}

selection::change::~change() {
	if (s_type == remove || s_type == clear)
		removed_regions.~region_pair();
	else
		added_region.~QRegion();
}
// end region_change
