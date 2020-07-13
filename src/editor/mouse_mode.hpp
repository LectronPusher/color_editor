#pragma once

#include "image/image_view.hpp"

#include <QCheckBox>

namespace editor {

class mouse_mode : public QCheckBox {
	Q_OBJECT
	
public:
	enum mode {pan, color, point, none};
	
	mouse_mode(mode mode_in, QString text);
	static void set_view(image::image_view *new_view);
	static void set(mode new_mode);
	static mode current();
	
public slots:
	void maybe_set(int state);
	void update_checked_state();
	
signals:
	void mode_changed();
	
private:
	const mode local_mode;
	
	static mode current_mode;
	static image::image_view *view;
	static mouse_mode * first;
	
}; // mouse_mode_checkbox

} // editor
