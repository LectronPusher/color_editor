#include "image_base.hpp"

#include <QGraphicsScene>

namespace editor{
namespace image {

image_base::image_base(editor_model *model) : model(model) {}

QRectF image_base::boundingRect() const {
	return QRectF(model->image_rect());
}

void image_base::paint(QPainter *painter, const QStyleOptionGraphicsItem *,
					   QWidget *) {
	painter->drawImage(model->image_rect(), model->image());
	model->paint_on(painter, scene()->backgroundBrush());
}

} // image
} // editor
