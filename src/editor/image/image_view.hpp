#pragma once

#include "../editor_model.hpp"
#include "image_base.hpp"

#include <QGraphicsView>
#include <QMouseEvent>

namespace editor{
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
	
	void update_mouse_mode();
	
signals:
	void point_selected(QPoint point);
	void combine_points(int n);
	
protected:
	void mouseMoveEvent(QMouseEvent *event);
	void mousePressEvent(QMouseEvent *event);
	void mouseReleaseEvent(QMouseEvent *event);
	
private:
	const qreal scale_factor = 1.2;
	
	QPoint last_position;
	int mouse_move_count;
	
	QPoint scene_point(const QPoint &view_point);
	
}; // image_view

} // image
} // editor
