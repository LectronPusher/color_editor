#include "model_renderer.hpp"

#include <QGraphicsScene>

using namespace editor::image;

model_renderer::model_renderer(select::selection *&sel, QImage &img)
: selection(sel), source_img(img) {}

QRectF model_renderer::boundingRect() const {
	return source_img.rect();
}

void model_renderer::paint(QPainter *painter, const QStyleOptionGraphicsItem *,
						   QWidget *) {
	painter->setBackground(scene()->backgroundBrush());
	painter->drawImage(source_img.rect(), source_img);
	render_effect(painter, true);
}

void model_renderer::render_effect(QPainter *painter, bool use_background) {
	if (no_effective_effect()) return;
	
	painter->setClipRegion(selection->selected_region());
	switch (eff_state.p_mode) {
		case color::effect_state::replace:
			painter->setCompositionMode(QPainter::CompositionMode_Source);
			// if there is a background, paint it, then paint over it
			if (!use_background)
				break;
			painter->fillRect(selection->region_rect(), painter->background());
			// fall through
		case color::effect_state::over:
			painter->setCompositionMode(QPainter::CompositionMode_SourceOver);
	}
	painter->drawImage(source_img.rect(), effect);
}

bool model_renderer::no_effective_effect() {
	return (
		effect.isNull() ||
		selection == nullptr || // short circuit allows dereference
		selection->selected_region().isEmpty()
	);
}

void model_renderer::update_effect_state(const color::effect_state &new_state) {
	eff_state = new_state;
	create_effect_and_render();
}

void model_renderer::create_effect_and_render() {
	if (selection) {
		effect = color::create_effect(eff_state, source_img, selection->region_rect());
		update(selection->region_rect());
	}
}
