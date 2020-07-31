#include "editor_model.hpp"

#include <QDebug>

namespace editor {

const QImage editor_model::image() {
	return _image;
}

const QRect editor_model::image_rect() {
	return image().rect();
}

const QRegion editor_model::selected_region() {
	return combined;
}

const QRect editor_model::region_rect() {
	return selected_region().boundingRect();
}

const QSet<QRgb> editor_model::color_table() {
	return _color_table;
}

bool editor_model::is_altered() {
	return !applied_changes.empty();
}

void editor_model::set_image(const QImage &new_image) {
	clear_regions();
	applied_changes.clear();
	undone_changes.clear();
	_image = new_image;
	update_color_table();
}

void editor_model::update_color_table() {
	_color_table.clear();
	_color_table.reserve(1000);
	for (int row = 0; row < image().width(); ++row) {
		for (int col = 0; col < image().height(); ++col) {
			_color_table.insert(image().pixel(row, col));
		}
	}
	_color_table.squeeze();
}

void editor_model::set_mask(const mask_pair& pair) {
	mask = pair.first;
	mode = pair.second;
	emit contents_updated(region_rect());
}

const QImage editor_model::apply_mask() {
	QPainter painter(&_image);
	paint_on(&painter);
	mask = QImage();
	return _image;
}

void editor_model::paint_on(QPainter *painter, const QBrush &background) {
	if (!mask.isNull()) {
		painter->save();
		painter->setClipRegion(selected_region());
		if (mode == replace) {
			painter->setCompositionMode(QPainter::CompositionMode_Source);
			if (background != Qt::NoBrush) {
				painter->fillRect(region_rect(), background);
				painter->setCompositionMode(QPainter::CompositionMode_SourceOver);
			}
		}
		painter->drawImage(region_rect(), mask);
		painter->restore();
	}
}

void editor_model::add_region(const select_region &region_pair) {
	select_type s_type = region_pair.first;
	QRegion region = region_pair.second;
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
	add_region({clear, QRegion()});
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
			selected -= data.removed_regions.first;
			excluded -= data.removed_regions.second;
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
				selected |= data.removed_regions.first;
				excluded |= data.removed_regions.second;
				break;
			case clear:
				selected = data.removed_regions.first;
				excluded = data.removed_regions.second;
		}
		undone_changes.push_front(data);
		applied_changes.pop_front();
		update_combined();
	}
}

void editor_model::update_combined() {
	QRegion old = combined;
	combined = selected - excluded;
	QRect difference = (combined ^ old).boundingRect();
	if (!difference.isEmpty())
		emit contents_updated(difference);
	if (old.boundingRect() != combined.boundingRect())
		emit boundary_rect_updated(combined.boundingRect());
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
			parent.removed_regions.first |= child.removed_regions.first;
			parent.removed_regions.second |= child.removed_regions.second;
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

} // editor
