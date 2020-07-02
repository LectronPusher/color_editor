#include "selector_types.hpp"

namespace editor {
namespace select {
namespace selector_types {

// select_all
select_all::select_all(QWidget *parent) : selector(parent, "Select All") {
	select_nothing = new QCheckBox("Select Nothing", this);
	options->addWidget(select_nothing, 0, 0);
}

QRegion select_all::select(const QImage &image) const {
	if (select_nothing->isChecked())
		return QRegion();
	return QRegion(image.rect());
}
// end select_all

} // selector_types
} // select
} // editor
