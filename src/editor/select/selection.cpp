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
	return !past_changes.empty();
}

void selection::add_region(select_type s_type, const QRegion &region) {
	switch (s_type) {
		case select:
			add({s_type, region - selected});
			break;
		case exclude:
			add({s_type, region - excluded});
			break;
		case remove:
			if (past_changes.empty())
				return;
			add({s_type, {region & selected, region & excluded}});
			break;
		case clear:
			if (past_changes.empty() || (past_changes.top().s_type == clear))
				return;
			add({s_type, {selected, excluded}});
			break;
	}
	emit region_added();
}

void selection::apply(const change &change) {
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
	update_combined();
}

void selection::unapply(const change &change) {
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
	update_combined();
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
	if (past_changes.empty())
		return;
	change parent = std::move(past_changes.top());
	past_changes.pop();
	for (int i = 1; i < n; ++i) {
		const change &child = past_changes.top();
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
		past_changes.pop();
	}
	past_changes.push(parent);
}
