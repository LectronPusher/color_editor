#include "model_renderer.hpp"

#include <QGraphicsScene>

using namespace editor::image;

model_renderer::model_renderer(select::selection *&sel) : selection(sel) {}

QRectF model_renderer::boundingRect() const {
	return source.rect();
}

void model_renderer::paint(QPainter *painter, unused_args) {
	painter->setBackground(scene()->backgroundBrush());
	painter->drawImage(source.rect(), source);
	render_effect(painter, true);
}

void model_renderer::render_effect(QPainter *painter, bool use_background) {
	if (no_effective_effect()) return;
	
	painter->setClipRegion(selection->selected_region());
	switch (mode) {
		case painting_mode::replace:
			painter->setCompositionMode(QPainter::CompositionMode_Source);
			// if there is a background, paint it, then paint over it
			if (!use_background)
				break;
			painter->fillRect(selection->region_rect(), painter->background());
			// fall through
		case painting_mode::over:
			painter->setCompositionMode(QPainter::CompositionMode_SourceOver);
	}
	painter->drawImage(source.rect(), effect);
}

bool model_renderer::no_effective_effect() {
	return (
		effect.isNull() ||
		selection == nullptr || // short circuit allows dereference
		selection->selected_region().isEmpty()
	);
}

void model_renderer::update_effect(const QImage &new_effect) {
	if (effect != new_effect) {
		effect = new_effect;
		if (selection)
			update(selection->region_rect());
	}
}

void model_renderer::update_mode(painting_mode::mode new_mode) {
	if (mode != new_mode) {
		mode = new_mode;
		if (selection)
			update(selection->region_rect());
	}
}
