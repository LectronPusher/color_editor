#include "./image_base.hpp"

namespace editor{
namespace image {

image_base::image_base(QImage image_orig) {
	image = image_orig;
}

QRectF image_base::boundingRect() const {
	return QRectF(image.rect());
}

void image_base::paint(QPainter *painter, const QStyleOptionGraphicsItem *,
					   QWidget *) {
	painter->drawImage(image.rect(), image);
}

QImage image_base::get_image() {
	return image;
}


} // image
} // editor
