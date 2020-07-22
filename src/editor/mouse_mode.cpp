#include "mouse_mode.hpp"

#include <QDebug>

namespace editor {

mouse_mode::mode_enum mouse_mode::global_mode = mouse_mode::none;
int mouse_mode::index = -1;
image::image_view *mouse_mode::view = nullptr;
QList<mouse_mode *> mouse_mode::all_list = QList<mouse_mode *>();

mouse_mode::mouse_mode(mode_enum new_mode, QString text)
: QCheckBox(text), local_mode(new_mode), local_index(all_list.size()) {
	all_list.push_back(this);
	connect(this, &QCheckBox::stateChanged, this, &mouse_mode::maybe_set);
}

void mouse_mode::set_global_mode(mouse_mode::mode_enum new_mode) {
	if (global_mode != new_mode) {
		global_mode = new_mode;
		if (view != nullptr)
			view->update_mouse_mode();
	}
	if (new_mode == none && index != -1)
		all_list.at(index)->setCheckState(Qt::Unchecked);
}

mouse_mode::mode_enum mouse_mode::mode() {
	return global_mode;
}

void mouse_mode::set_view(image::image_view *new_view) {
	view = new_view;
}

void mouse_mode::hideEvent(QHideEvent *) {
	if (isChecked()) {
		unchecked_by_hiding = true;
		setChecked(false);
	}
}

void mouse_mode::showEvent(QShowEvent *) {
	if (unchecked_by_hiding) {
		unchecked_by_hiding = false;
		setChecked(true);
	}
}

void mouse_mode::maybe_set(int state) {
	if (state == Qt::Checked) {
		if (index != -1 && index != local_index)
			all_list.at(index)->setCheckState(Qt::Unchecked);
		index = local_index;
		set_global_mode(local_mode);
	} else if (state == Qt::Unchecked) {
		if (index == local_index) {
			index = -1;	
			set_global_mode(none);
		}
	}
}

} // editor
