#include "image_base.hpp"

#include <QGraphicsScene>
#include <QDebug>

namespace editor{
namespace image {

image_base::image_base(const QImage image_orig, QGraphicsItem *parent)
: QGraphicsItem(parent) {
	stored_image = image_orig.convertToFormat(QImage::Format_ARGB32);
}

QRectF image_base::boundingRect() const {
	return QRectF(stored_image.rect());
}

void image_base::paint(QPainter *painter, const QStyleOptionGraphicsItem *,
					   QWidget *) {
	painter->drawImage(stored_image.rect(), stored_image);
	image_mask.paint_on(painter, scene()->backgroundBrush());
}

const QImage image_base::image() const {
	return stored_image;
}

bool image_base::has_image() const {
	return !stored_image.isNull();
}

bool image_base::is_modified() const {
	return image_mask.is_valid() || mask_applied;
}

const QImage image_base::apply_mask() {
	if (image_mask.is_valid()) {
		mask_applied = true;
		QPainter painter(&stored_image);
		image_mask.paint_on(&painter);
		image_mask = mask();
	}
	return stored_image;
}

void image_base::set_mask(const mask &new_mask) {
	image_mask = new_mask;
	scene()->update(new_mask.rect());
}

} // image
} // editor
