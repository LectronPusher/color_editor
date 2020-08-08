#include "editor_model.hpp"

using namespace editor;

const QImage editor_model::source_image() {
	return image;
}

const QRect editor_model::image_rect() {
	return image.rect();
}

const QRegion editor_model::selected_region() {
	return combined;
}

const QRect editor_model::region_rect() {
	return combined.boundingRect();
}

bool editor_model::is_altered() {
	return !applied_changes.empty();
}

void editor_model::set_image(const QImage &new_image) {
	clear_regions();
	applied_changes.clear();
	undone_changes.clear();
	image = new_image.convertToFormat(QImage::Format_ARGB32);
	emit image_changed(image);
}

void editor_model::set_mask(const QImage& new_mask, painting_mode::mode new_mode) {
	bool mask_changed = mask != new_mask;
	bool mode_changed = mode != new_mode;
	if (mask_changed)
		mask = new_mask;
	if (mode_changed)
		mode = new_mode;
	if (mask_changed || mode_changed)
		emit contents_updated(region_rect());
}

void editor_model::apply_mask() {
	QPainter painter(&image);
	draw_mask(&painter, false);
	clear_regions();
	applied_changes.clear();
	undone_changes.clear();
	emit image_changed(image);
}

void editor_model::draw_mask(QPainter *painter, bool use_background) {
	if (!mask.isNull()) {
		painter->save();
		painter->setClipRegion(selected_region());
		switch (mode) {
			case painting_mode::replace:
				painter->setCompositionMode(QPainter::CompositionMode_Source);
				// if there is a background, paint it, then paint over it
				if (!use_background)
					break;
				painter->fillRect(region_rect(), painter->background());
				// fall through
			case painting_mode::over:
				painter->setCompositionMode(QPainter::CompositionMode_SourceOver);
		}
		painter->drawImage(region_rect(), mask);
		painter->restore();
	}
}

void editor_model::add_region(const QRegion &region, select_type s_type) {
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

void editor_model::clear_regions() {
	add_region(QRegion(), clear);
}

void editor_model::apply_change(const region_change &data) {
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

void editor_model::update_combined() {
	QRegion old = combined;
	combined = selected - excluded;
	QRect difference = (combined ^ old).boundingRect();
	if (!difference.isEmpty())
		emit contents_updated(difference);
	if (old.boundingRect() != combined.boundingRect())
		emit region_boundary_updated(combined.boundingRect());
}

void editor_model::undo() {
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

void editor_model::redo() {
	if (!undone_changes.empty()) {
		apply_change(undone_changes.front());
		undone_changes.pop_front();
	}
}

void editor_model::combine_recent_changes(int n) {
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
editor_model::region_change::region_change(select_type type, QRegion region)
: s_type(type), added_region(region) {}

editor_model::region_change::region_change(select_type type, region_pair regions)
: s_type(type), removed_regions(regions) {}

editor_model::region_change::region_change(const region_change &other) {
	s_type = other.s_type;
	if (s_type == remove || s_type == clear)
		new(&removed_regions) region_pair(other.removed_regions);
	else
		new(&added_region) QRegion(other.added_region);
}

editor_model::region_change editor_model::region_change::operator=(const region_change &other) {
	s_type = other.s_type;
	if (s_type == remove || s_type == clear)
		new(&removed_regions) region_pair(other.removed_regions);
	else
		new(&added_region) QRegion(other.added_region);
	return *this;
}

editor_model::region_change::~region_change() {
	if (s_type == remove || s_type == clear)
		removed_regions.~region_pair();
	else
		added_region.~QRegion();
}
// end region_change
