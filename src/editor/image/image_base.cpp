#include "image_base.hpp"

#include <QGraphicsScene>

namespace editor{
namespace image {

image_base::image_base(const QImage image_orig, QGraphicsItem *parent)
: QGraphicsItem(parent), stored_image(image_orig) {}

QRectF image_base::boundingRect() const {
	return QRectF(stored_image.rect());
}

void image_base::paint(QPainter *painter, const QStyleOptionGraphicsItem *,
					   QWidget *) {
	painter->drawImage(stored_image.rect(), stored_image);
	if (!mask.isNull()) {
		painter->setClipRegion(mask_region);
		painter->drawImage(mask_region.boundingRect(), mask);
	}
}

const QImage image_base::image() const {
	return stored_image;
}

bool image_base::has_image() {
	return !stored_image.isNull();
}

bool image_base::is_modified() {
	return modified;
}

const QImage image_base::apply_mask() {
	if (!mask.isNull()) {
		QPainter painter(&stored_image);
		painter.setClipRegion(mask_region);
		painter.drawImage(mask_region.boundingRect(), mask);
	}
	return stored_image;
}

void image_base::set_mask(const QImage &new_mask, const QRegion &region) {
	if (!new_mask.isNull()) modified = true;
	mask = new_mask;
	mask_region = region;
	scene()->update(mask_region.boundingRect());
}

} // image
} // editor
