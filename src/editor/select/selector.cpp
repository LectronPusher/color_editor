#include "selector.hpp"

namespace editor {
namespace select {

editor_model *selector::model = nullptr;

selector::selector(QString name_in) : selector_name(name_in) {
	options = new QVBoxLayout;
	setLayout(options);
}

const QString selector::name() const {
	return selector_name;
}

void selector::set_model(editor_model *new_model) {
	model = new_model;
}

void selector::point_selected(const QPoint &) {}

} // select
} // editor
