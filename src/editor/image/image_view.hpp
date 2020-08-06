#pragma once

#include "../mouse_mode.hpp"

#include <QGraphicsView>
#include <QMouseEvent>

namespace editor {
namespace image {

class image_view : public QGraphicsView {
	Q_OBJECT
	
public:
	image_view(QWidget *parent = nullptr);
	
public slots:
	void reset_view_rect(const QRect &rect);
	void redraw_rect(const QRect &scene_rect);
	
	void zoom_in();
	void zoom_out();
	void reset_zoom();
	
	void set_mouse_mode(mouse_mode::mode new_mode);
	
signals:
	void point_selected(QPoint point);
	void combine_points(int n);
	
protected:
	void mouseMoveEvent(QMouseEvent *event);
	void mousePressEvent(QMouseEvent *event);
	void mouseReleaseEvent(QMouseEvent *event);
	
private:
	const qreal scale_factor = 1.2;
	
	mouse_mode::mode mode = mouse_mode::none;
	
	QPoint last_position;
	int movement_count;
	
	QPoint scene_point(const QPoint &view_point);
	
}; // image_view

} // image
} // editor
