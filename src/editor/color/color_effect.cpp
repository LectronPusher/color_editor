#include "color_effect.hpp"

namespace editor {
namespace color {

color_effect::color_effect(QWidget *parent, QString name_in)
: QWidget(parent), name(name_in){
	options = new QGridLayout;
	options->setContentsMargins(0, 0, 0, 0);
	setLayout(options);
}

} // color
} // editor
