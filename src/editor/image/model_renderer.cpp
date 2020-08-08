#include "model_renderer.hpp"

#include <QGraphicsScene>

using namespace editor::image;

model_renderer::model_renderer(editor_model *model) : model(model) {}

QRectF model_renderer::boundingRect() const {
	return model->image_rect();
}

void model_renderer::paint(QPainter *painter, const QStyleOptionGraphicsItem *,
					   QWidget *) {
	painter->setBackground(scene()->backgroundBrush());
	painter->drawImage(model->image_rect(), model->source_image());
	model->draw_mask(painter, true);
}
