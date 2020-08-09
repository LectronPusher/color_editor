#include "effect.hpp"

using namespace editor::color;

effect::effect(QString name_in) : effect_name(name_in) {
	options = new QVBoxLayout;
	setLayout(options);
}

const QString effect::name() const {
	return effect_name;
}

void effect::set_mode(painting_mode::mode new_mode) {
	emit mode_changed(mode = new_mode, {});
}

void effect::showEvent(QShowEvent *event) {
	QWidget::showEvent(event);
	emit mode_changed(mode, {});
	emit altered();
}
