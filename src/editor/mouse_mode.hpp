#pragma once

#include "image/image_view.hpp"

#include <QCheckBox>

namespace editor {

class mouse_mode : public QCheckBox {
	Q_OBJECT
	
public:
	enum mode_enum {none, pan, color, point};
	
	mouse_mode(mode_enum mode_in, QString text);
	static void set_global_mode(mode_enum new_mode);
	static mode_enum mode();
	static void set_view(image::image_view *new_view);
	
public slots:
	void maybe_set(int state);
	void update_checked_state();
	
signals:
	void global_mode_changed();
	
protected:
	void hideEvent(QHideEvent *event) override;
	
private:
	const mode_enum local_mode;
	
	static mode_enum global_mode;
	static image::image_view *view;
	static mouse_mode * first;
	
}; // mouse_mode

} // editor
