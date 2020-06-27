#include "selector_stack.hpp"

namespace editor {
namespace select {

void selector_stack::add_selector(selector *sel) {
	addWidget(sel);
}

selector *selector_stack::active_selector() {
	return static_cast<selector *>(currentWidget());
}

void selector_stack::setup_with_combo_box(QComboBox *combo_box) {
	connect(combo_box, QOverload<int>::of(&QComboBox::currentIndexChanged),
			this, &QStackedWidget::setCurrentIndex);
	for (int i = 0; i < count(); ++i) {
		selector *sel = static_cast<selector *>(widget(i));
		combo_box->addItem(sel->name);
	}
}

// Qt::Orientations selector_stack::expandingDirections() const {
// 	return Qt::Vertical & Qt::Horizontal;
// }

} // select
} // editor
