#include "image_base.hpp"

#include <QGraphicsScene>

using namespace editor::image;

image_base::image_base(editor_model *model) : model(model) {}

QRectF image_base::boundingRect() const {
	return model->image_rect();
}

void image_base::paint(QPainter *painter, const QStyleOptionGraphicsItem *,
					   QWidget *) {
	painter->drawImage(model->image_rect(), model->source_image());
	model->draw_mask(painter, scene()->backgroundBrush());
}
