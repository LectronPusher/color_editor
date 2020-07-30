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
	QRegion region = region_pair.first;
	select_type s_type = region_pair.second;
	switch (s_type) {
		case select:
			apply_change({s_type, region - selected});
			break;
		case exclude:
			apply_change({s_type, region - excluded});
			break;
		case remove:
			apply_change({s_type, {region - selected, region - excluded}});
			break;
		case clear:
			apply_change({s_type, {selected, excluded}});
	}
	undone_changes.clear();
}

void editor_model::clear_regions() {
	add_region({QRegion(), clear});
}

void editor_model::apply_change(const undo_data &data) {
	qDebug("applying");
	switch (data.s_type) {
		case select:
			selected |= data.added_region;
			break;
		case exclude:
			excluded |= data.added_region;
			break;
		case remove:
			selected -= data.removed_regions.first;
			excluded -= data.removed_regions.second;
			break;
		case clear:
			selected = QRegion();
			excluded = QRegion();
	}
	applied_changes.push_back(data);
	update_combined();
}

void editor_model::undo() {
	qDebug("undo");
	if (!applied_changes.empty()) {
		qDebug("not empty");
		const undo_data &data = applied_changes.back();
		qDebug("undoing");
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
		undone_changes.push_back(data);
		applied_changes.pop_back();
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
	qDebug("redo");
	if (!undone_changes.empty()) {
		qDebug("not empty");
		apply_change(undone_changes.back());
		undone_changes.pop_back();
	}
}

void editor_model::combine_recent_undos(int n) {
	int size = applied_changes.size();
	if (size >= n) {
		for (int i = 0; i < n; ++i) {
			; // TODO
		}
	}
}

editor_model::undo_data::undo_data(select_type type, QRegion region)
: s_type(type), added_region(region) {}

editor_model::undo_data::undo_data(select_type type, region_pair regions)
: s_type(type), removed_regions(regions) {}

editor_model::undo_data::undo_data(const undo_data &other) {
	s_type = other.s_type;
	if (s_type == remove || s_type == clear)
		new(&removed_regions) region_pair(other.removed_regions);
	else
		new(&added_region) QRegion(other.added_region);
}

editor_model::undo_data editor_model::undo_data::operator=(const undo_data &other) {
	s_type = other.s_type;
	if (s_type == remove || s_type == clear)
		new(&removed_regions) region_pair(other.removed_regions);
	else
		new(&added_region) QRegion(other.added_region);
	return *this;
}

editor_model::undo_data::~undo_data() {
	if (s_type == remove || s_type == clear)
		removed_regions.~region_pair();
	else
		added_region.~QRegion();
}

} // editor
