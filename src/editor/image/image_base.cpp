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
	if (!mask.isNull())
		painter->drawImage(QRect(mask_top_left, mask.size()), mask);
}

const QImage image_base::get_image() const {
	return image;
}

void image_base::set_color_mask(const QImage color_mask, const QPoint initial_point) {
	mask = color_mask;
	mask_top_left = initial_point;
}

const QImage image_base::apply_mask() {
	if (!mask.isNull()) {
		QPainter painter(&image);
		painter.drawImage(QRect(mask_top_left, mask.size()), mask);
	}
	return image;
}

} // image
} // editor
