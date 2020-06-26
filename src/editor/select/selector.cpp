#include "selector.hpp"

namespace editor {
namespace select {

selector::selector(QWidget *parent, QString name_in) 
: QWidget(parent), name(name_in) {
	options_box = new QVBoxLayout;
	setLayout(options_box);
}

// select_all
select_all::select_all(QWidget *parent) : selector(parent, "Select All") {
	select_nothing = new QCheckBox("Select Nothing", this);
	options_box->addWidget(select_nothing);
	setLayout(options_box);
}

point_set select_all::make_selection(const QImage &image) const {
	point_set points;
	if (!select_nothing->isChecked()) {
		for (int x = 0; x < image.width(); ++x) {
			for (int y = 0; y < image.height(); ++y) {
				points.insert(QPoint(x, y));
			}
		}
	}
	return points;
}
// end select_all

} // select
} // editor
