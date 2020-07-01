#include "selector.hpp"

namespace editor {
namespace select {

selector::selector(QWidget *parent, QString name_in)
: QWidget(parent), name(name_in){
	options = new QGridLayout;
	options->setContentsMargins(0, 0, 0, 0);
	setLayout(options);
}

} // select
} // editor
