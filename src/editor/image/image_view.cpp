#include "image_view.hpp"
#include "circle_cursor.hpp"

using namespace editor::image;

image_view::image_view(QWidget *parent)
: QGraphicsView(parent) {
	setScene(new QGraphicsScene(this));
	scene()->setBackgroundBrush(Qt::darkGray);
}

void image_view::scale_down_to_fit(const QRect &rect) {
	reset_zoom();
	// scale down the image if larger than the viewport, handle scrollbar bug
	QSize v_size = viewport()->size();
	if (rect.width() > v_size.width() || rect.height() > v_size.height()) {
		setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
		setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
		
		fitInView(rect, Qt::KeepAspectRatio);
		
		setHorizontalScrollBarPolicy(Qt::ScrollBarAsNeeded);
		setVerticalScrollBarPolicy(Qt::ScrollBarAsNeeded);
	}
}

void image_view::redraw_rect(const QRect &scene_rect) {
	scene()->update(scene_rect);
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

void image_view::maybe_set_mouse_mode(int id, bool checked) {
	if (checked) {
		switch (id) {
			case mouse_mode::none:
			case mouse_mode::pan:
			case mouse_mode::single_point:
			case mouse_mode::combined_points:
				set_mouse_mode((mouse_mode::mode)id);
			default:
				break;
		}
	}
}

void image_view::set_mouse_mode(mouse_mode::mode new_mode) {
	const static QCursor custom_cursor = circle_cursor();
	
	if (mode != new_mode) {
		mode = new_mode;
		switch (mode) {
			case mouse_mode::none:
				setDragMode(QGraphicsView::NoDrag);
				viewport()->setCursor(Qt::ArrowCursor);
				break;
			case mouse_mode::pan:
				setDragMode(QGraphicsView::ScrollHandDrag);
				break;
			case mouse_mode::single_point:
			case mouse_mode::combined_points:
				setDragMode(QGraphicsView::NoDrag);
				viewport()->setCursor(custom_cursor);
			default:
				break;
		}
	}
}

void image_view::mouseMoveEvent(QMouseEvent *event) {
	if (mode == mouse_mode::combined_points) {
		event->accept();
		QPoint position = scene_point(event->pos());
		if (position != last_position) {
			++movement_count;
			last_position = position;
			emit point_selected(position);
		}
	}
	QGraphicsView::mouseMoveEvent(event);
}

void image_view::mousePressEvent(QMouseEvent *event) {
	if (event->button() == Qt::LeftButton) {
		QPoint position = scene_point(event->pos());
		switch (mode) {
			case mouse_mode::combined_points:
				movement_count = 1;
				last_position = position;
				// fall through
			case mouse_mode::single_point:
				emit point_selected(position);
			default:
				break;
		}
	}
	QGraphicsView::mousePressEvent(event);
}

void image_view::mouseReleaseEvent(QMouseEvent *event) {
	if (event->button() == Qt::LeftButton) {
		if (mode == mouse_mode::combined_points) {
			event->accept();
			emit combine_points(movement_count);
			movement_count = 0;
			last_position = QPoint();
		}
	}
	QGraphicsView::mouseReleaseEvent(event);
}

QPoint image_view::scene_point(const QPoint &view_point) {
	QPointF pf = mapToScene(view_point);
	// QPointF::toPoint() rounds to the nearest value, I'd rather always round down
	return {(int)pf.x(), (int)pf.y()};
}
