#include "effect_widget.hpp"

using namespace editor::color;

effect_widget::effect_widget(QString name_in) : effect_name(name_in) {
	options = new QVBoxLayout;
	setLayout(options);
}

const QString effect_widget::name() const {
	return effect_name;
}

void effect_widget::showEvent(QShowEvent *event) {
	QWidget::showEvent(event);
	emit altered(get_state());
}
