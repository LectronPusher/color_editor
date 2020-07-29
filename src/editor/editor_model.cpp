#include "editor_model.hpp"

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
	return image_altered;
}

void editor_model::set_image(const QImage &new_image) {
	_image = new_image;
	image_altered = false;
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
	if (!mask.isNull())
		image_altered = true;
}

void editor_model::add_region(const select_region &region_pair) {
	QRegion region = region_pair.first;
	switch (region_pair.second) {
		case select:
			selected |= region;
			break;
		case exclude:
			excluded |= region;
			break;
		case remove:
			excluded -= region;
			selected -= region;
			break;
	}
	combined = selected - excluded;
}

void editor_model::clear_regions() {
	selected = QRegion();
	excluded = QRegion();
	combined = QRegion();
	mask = QImage();
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

} // editor
