#include "./image_base.hpp"

namespace editor{
namespace image {

image_base::image_base(const QImage image_orig, QGraphicsItem *parent) 
: QGraphicsItem(parent), image(image_orig) {
}

QRectF image_base::boundingRect() const {
	return QRectF(image.rect());
}

void image_base::paint(QPainter *painter, const QStyleOptionGraphicsItem *,
					   QWidget *) {
	painter->drawImage(image.rect(), image);
	if (!mask.isNull()) {
		painter->setClipRegion(mask_region);
		painter->drawImage(mask_region.boundingRect(), mask);
	}
}

const QImage image_base::get_image() const {
	return image;
}

void image_base::set_mask(const QImage new_mask, const QRegion region) {
	mask = new_mask;
	mask_region = region;
}

const QImage image_base::apply_mask() {
	if (!mask.isNull()) {
		QPainter painter(&image);
		painter.setClipRegion(mask_region);
		painter.drawImage(mask_region.boundingRect(), mask);
	}
	return image;
}

} // image
} // editor
