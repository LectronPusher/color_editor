#include "selector.hpp"

namespace editor {
namespace select {

QImage selector::image;
QSet<QRgb> selector::color_table;

selector::selector(QString name_in) : selector_name(name_in) {
	options = new QVBoxLayout;
	setLayout(options);
}

const QString selector::name() const {
	return selector_name;
}

void selector::update_image(const QImage &new_image) {
	image = new_image;
	update_color_table();
}

void selector::update_color_table() {
	color_table.clear();
	color_table.reserve(1000);
	for (int row = 0; row < image.width(); ++row) {
		for (int col = 0; col < image.height(); ++col) {
			color_table.insert(image.pixel(row, col));
		}
	}
	color_table.squeeze();
}

} // select
} // editor
