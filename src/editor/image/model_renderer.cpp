#include "model_renderer.hpp"

#include <QGraphicsScene>

using namespace editor::image;

model_renderer::model_renderer(editor_model *model) : model(model) {}

QRectF model_renderer::boundingRect() const {
	return model->image_rect();
}

void model_renderer::paint(QPainter *painter, unused_args) {
	painter->setBackground(scene()->backgroundBrush());
	render(painter, true);
}

void model_renderer::render(QPainter *painter, bool use_background) {
	painter->drawImage(model->image_rect(), model->source_image());
	if (!mask.isNull()) {
		painter->setClipRegion(model->selected_region());
		switch (mode) {
			case painting_mode::replace:
				painter->setCompositionMode(QPainter::CompositionMode_Source);
				// if there is a background, paint it, then paint over it
				if (!use_background)
					break;
				painter->fillRect(model->region_rect(), painter->background());
				// fall through
			case painting_mode::over:
				painter->setCompositionMode(QPainter::CompositionMode_SourceOver);
		}
		painter->drawImage(model->image_rect(), mask);
	}
}
