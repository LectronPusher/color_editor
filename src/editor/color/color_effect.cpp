#include "color_effect.hpp"

namespace editor {
namespace color {

color_effect::color_effect(QWidget *parent, QString name_in)
: QWidget(parent), effect_name(name_in){
	options = new QGridLayout;
	setLayout(options);
}

const QString color_effect::name()  const {
	return effect_name;
}

} // color
} // editor
