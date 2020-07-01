#include "selector_types.hpp"

namespace editor {
namespace select {
namespace selector_types {

// select_all
select_all::select_all(QWidget *parent) : selector(parent, "Select All") {
	select_nothing = new QCheckBox("Select Nothing", this);
	options->addWidget(select_nothing, 0, 0);
}

point_set select_all::select(const QImage &image) const {
	point_set points;
	if (!select_nothing->isChecked()) {
		for (int x = 0; x <= image.width(); ++x) {
			// negative y, top left is start
			for (int y = 0; y >= -image.height(); --y) {
				points.insert(QPoint(x, y));
			}
		}
	}
	return points;
}
// end select_all

} // selector_types
} // select
} // editor
