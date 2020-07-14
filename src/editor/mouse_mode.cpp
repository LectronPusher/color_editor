#include "mouse_mode.hpp"

namespace editor {

mouse_mode::mode_enum mouse_mode::global_mode = mouse_mode::none;
image::image_view *mouse_mode::view = nullptr;
mouse_mode *mouse_mode::first = nullptr;

mouse_mode::mouse_mode(mode_enum new_mode, QString text)
: QCheckBox(nullptr), local_mode(new_mode) {
	setText(text);
	update_checked_state();
	
	connect(this, &QCheckBox::stateChanged, this, &mouse_mode::maybe_set);
	
	if (first == nullptr)
		first = this;
	
	connect(first, &mouse_mode::global_mode_changed,
			this, &mouse_mode::update_checked_state);
}

void mouse_mode::set_global_mode(mouse_mode::mode_enum new_mode) {
	global_mode = new_mode;
	if (view)
		view->update_mouse_mode();
	if (first)
		emit first->global_mode_changed();
}

mouse_mode::mode_enum mouse_mode::mode() {
	return global_mode;
}

void mouse_mode::set_view(image::image_view *new_view) {
	view = new_view;
}

void mouse_mode::maybe_set(int state) {
	if (state == Qt::Checked && mode() != local_mode)
		set_global_mode(local_mode);
	else if (state == Qt::Unchecked && mode() == local_mode)
		set_global_mode(none);
}

void mouse_mode::update_checked_state() {
	setChecked(mode() == local_mode && isVisible());
}

void mouse_mode::hideEvent(QHideEvent *event) {
	if (mode() == local_mode)
		set_global_mode(none);
}

} // editor
