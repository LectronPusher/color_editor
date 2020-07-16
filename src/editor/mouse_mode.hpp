#pragma once

#include "image/image_view.hpp"

#include <QCheckBox>
#include <QList>

namespace editor {

class mouse_mode : public QCheckBox {
	Q_OBJECT
	
public:
	enum mode_enum {none, pan, point, single_point};
	
	mouse_mode(mode_enum mode_in, QString text);
	static void set_global_mode(mode_enum new_mode);
	static mode_enum mode();
	static void set_view(image::image_view *new_view);
	
private slots:
	void maybe_set(int state);
	
private:
	const mode_enum local_mode;
	const int local_index;
	
	static mode_enum global_mode;
	static int index;
	static image::image_view *view;
	static QList<mouse_mode *> all_list;
	
}; // mouse_mode

} // editor
