#pragma once

#include "point_set.hpp"
#include "image/image_view.hpp"
#include "select/select_panel.hpp"
#include <QWidget>

namespace editor {

class main_window : public QWidget {
	Q_OBJECT
	
public:
	main_window(QWidget *parent = nullptr);
	
private slots:
	void select_points();
	
signals:
	void points_selected(point_set points);
	
private:
 	image::image_view *view;
// 	color::color_panel *color_pan;
	select::select_panel *select_pan;
	
}; // main_window

} // editor
