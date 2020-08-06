#include "effect.hpp"

using namespace editor::color;

effect::effect(QString name_in) : effect_name(name_in) {
	options = new QVBoxLayout;
	setLayout(options);
}

const QString effect::name()  const {
	return effect_name;
}

editor::editor_model::painting_mode effect::paint_mode() {
	return editor_model::default_mode;
}

void effect::showEvent(QShowEvent *event) {
	QWidget::showEvent(event);
	emit altered();
}
