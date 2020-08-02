#include "image_view.hpp"
#include "../mouse_mode.hpp"
#include "circle_cursor.hpp"

namespace editor{
namespace image {

image_view::image_view(QWidget *parent)
: QGraphicsView(parent) {
	setScene(new QGraphicsScene(this));
	scene()->setBackgroundBrush(Qt::darkGray);
	
	update_mouse_mode();
}

void image_view::reset_view_rect(const QRect &rect) {
	reset_zoom();
	scene()->setSceneRect(rect);
	
	// scale down the image if larger than the viewport, handle scrollbar bug
	QSize v_size = viewport()->size();
	if (rect.width() > v_size.width() || rect.height() > v_size.height()) {
		setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
		setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
		
		fitInView(sceneRect(), Qt::KeepAspectRatio);
		
		setHorizontalScrollBarPolicy(Qt::ScrollBarAsNeeded);
		setVerticalScrollBarPolicy(Qt::ScrollBarAsNeeded);
	}
}

void image_view::redraw_rect(const QRect &scene_rect) {
	updateScene({scene_rect});
}

void image_view::zoom_in() {
	scale(scale_factor, scale_factor);
}

void image_view::zoom_out() {
	scale(1/ scale_factor, 1/ scale_factor);
}

void image_view::reset_zoom() {
	setTransform(QTransform());
}

void image_view::update_mouse_mode() {
	static QCursor custom_cursor = circle_cursor();
	
	switch (mouse_mode::mode()) {
		case mouse_mode::none:
			setDragMode(QGraphicsView::NoDrag);
			viewport()->setCursor(Qt::ArrowCursor);
			break;
		case mouse_mode::pan:
			// TODO pan not working
			setDragMode(QGraphicsView::ScrollHandDrag);
			break;
		case mouse_mode::single_point:
		case mouse_mode::combined_points:
			setDragMode(QGraphicsView::NoDrag);
			viewport()->setCursor(custom_cursor);
	}
}

void image_view::mouseMoveEvent(QMouseEvent *event) {
	if (mouse_mode::mode() == mouse_mode::combined_points) {
		event->accept();
		QPoint position = scene_point(event->pos());
		if (position != last_position) {
			++mouse_move_count;
			last_position = position;
			emit point_selected(position);
		}
	}
}

void image_view::mousePressEvent(QMouseEvent *event) {
	if (event->button() == Qt::LeftButton) {
		QPoint position = scene_point(event->pos());
		switch (mouse_mode::mode()) {
			case mouse_mode::combined_points:
				mouse_move_count = 1;
				last_position = position;
				// fall through
			case mouse_mode::single_point:
				emit point_selected(position);
			default:
				break;
		}
	}
}

void image_view::mouseReleaseEvent(QMouseEvent *event) {
	if (event->button() == Qt::LeftButton) {
		if (mouse_mode::mode() == mouse_mode::combined_points) {
			event->accept();
			emit combine_points(mouse_move_count);
			mouse_move_count = 0;
			last_position = QPoint();
		}
	}
}

QPoint image_view::scene_point(const QPoint &view_point) {
	QPointF pf = mapToScene(view_point);
	// QPointF::toPoint() rounds to the nearest value, I'd rather always round down
	return QPoint((int)pf.x(), (int)pf.y());
}

} // image
} // editor
