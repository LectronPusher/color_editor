#include "effect.hpp"

namespace editor {
namespace color {

effect::effect(QString name_in) : effect_name(name_in) {
	options = new QGridLayout;
	setLayout(options);
}

const QString effect::name()  const {
	return effect_name;
}

void effect::showEvent(QShowEvent *event) {
	QWidget::showEvent(event);
	emit altered();
}


} // color
} // editor
