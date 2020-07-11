#include "selector.hpp"

namespace editor {
namespace select {

selector::selector(QWidget *parent, QString name_in)
: QWidget(parent), selector_name(name_in){
	options = new QGridLayout;
	setLayout(options);
}

const QString selector::name() const {
	return selector_name;
}

} // select
} // editor
