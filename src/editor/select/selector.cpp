#include "selector.hpp"

namespace editor {
namespace select {

QImage selector::image = QImage();

selector::selector(QString name_in) : selector_name(name_in) {
	options = new QGridLayout;
	setLayout(options);
}

const QString selector::name() const {
	return selector_name;
}

void selector::update_image(const QImage &new_image) {
	image = new_image;
}

void selector::point_selected(const QPoint &) {}

} // select
} // editor
