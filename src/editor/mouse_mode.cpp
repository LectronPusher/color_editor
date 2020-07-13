#include "mouse_mode.hpp"

namespace editor {

mouse_mode::mode mouse_mode::current_mode = mouse_mode::none;
image::image_view *mouse_mode::view = nullptr;
mouse_mode * mouse_mode::first = nullptr;

mouse_mode::mouse_mode(mode new_mode, QString text)
: QCheckBox(nullptr), local_mode(new_mode) {
	setText(text);
	update_checked_state();
	
	connect(this, &QCheckBox::stateChanged, this, &mouse_mode::maybe_set);
	
	if (first == nullptr)
		first = this;
	else
		connect(first, &mouse_mode::mode_changed,
				this, &mouse_mode::update_checked_state);
}

mouse_mode::mode mouse_mode::current() {
	return current_mode;
}

void mouse_mode::set(mouse_mode::mode new_mode) {
	current_mode = new_mode;
	first->update_checked_state();
	view->update_mouse_mode();
	emit first->mode_changed();
}

void mouse_mode::set_view(image::image_view *new_view) {
	view = new_view;
}

void mouse_mode::maybe_set(int state) {
	if (state == Qt::Checked && current() != local_mode)
		set(local_mode);
	else if (state == Qt::Unchecked && current() == local_mode)
		set(none);
}

void mouse_mode::update_checked_state() {
	setChecked(current() == local_mode);
}

} // editor
