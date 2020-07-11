#include "selector.hpp"

namespace editor {
namespace select {

selector::selector(QString name_in) : selector_name(name_in) {
	options = new QGridLayout;
	setLayout(options);
}

const QString selector::name() const {
	return selector_name;
}

} // select
} // editor
