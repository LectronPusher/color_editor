#include "image_view.hpp"
#include "../mouse_mode.hpp"

#include <QFileDialog>
#include <QMessageBox>
#include <QTextStream>
#include <QCursor>
#include <QBitmap>
#include <QPainter>
#include <QDebug>

namespace editor{
namespace image {

image_view::image_view(editor_model *model, QWidget *parent)
: QGraphicsView(parent), model(model) {
	setScene(new QGraphicsScene(this));
	scene()->setBackgroundBrush(Qt::darkGray);
	scene()->addItem(new image_base(model));
	
	update_mouse_mode();
}

bool image_view::modifications_resolved() {
	if (model->is_altered()) {
		auto dialog_ans = QMessageBox::warning(
			this,
			"Unsaved Changes",
			"The image has been modified.\nDo you want to save your changes?",
			QMessageBox::Save | QMessageBox::Discard | QMessageBox::Cancel
		);
		if (dialog_ans == QMessageBox::Cancel)
			return false;
		else if (dialog_ans == QMessageBox::Save)
			save_as();
	}
	return true;
}

void image_view::open_image(QString filepath) {
	if (modifications_resolved()) {
		if (filepath.isEmpty()) {
			filepath = QFileDialog::getOpenFileName(
				this,
				"Open Image",
				old_file.dir().path(),
				"Image Files (*.png *.jpg *.bmp)"
			);
		}
		if (!filepath.isEmpty()) {
			old_file = QFileInfo(filepath);
			QImage image = QImage(filepath);
			if (!image.isNull())
				set_image(image);
			else
				QTextStream(stdout) << "loading failed:" << filepath << Qt::endl;
		}
	}
}

void image_view::save_as() {
	if (!model->image().isNull()) {
		QString filepath = QFileDialog::getSaveFileName(
			this,
			"Save As",
			old_file.filePath(),
			"Image Files (*.png *.jpg *.bmp);;All Files (*)"
		);
		if (!filepath.isEmpty()) {
			QImage image = model->apply_mask();
			image.save(filepath);
			set_image(image);
			old_file = QFileInfo(filepath);
		}
	}
}

void image_view::update_rect(const QRect &scene_rect) {
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

// creates a mostly transparent cursor of 3 circles, inner 2 black, outer white
QCursor setup_circle_cursor() {
	QPoint center(16, 16);
	int r = 8;
	
	QBitmap mask = QBitmap(32, 32);
	mask.clear();
	QPainter painter(&mask);
	painter.setPen(Qt::color1);
	// idk why, but a QPen of width 3 doesn't work properly, oh well
	painter.drawEllipse(center, r + 1, r + 1);
	painter.drawEllipse(center, r, r);
	painter.drawEllipse(center, r - 1, r - 1);
	// the circles don't match perfectly, need to fill these 4 points:
	painter.drawPoint(center + QPoint(6, 6));
	painter.drawPoint(center + QPoint(6, -6));
	painter.drawPoint(center + QPoint(-6, 6));
	painter.drawPoint(center + QPoint(-6, -6));
	
	QBitmap pixels = QBitmap(32, 32);
	pixels.clear();
	QPainter painter2(&pixels);
	painter2.setPen(Qt::color1);
	painter2.drawEllipse(center, r, r);
	painter2.drawEllipse(center, r - 1, r - 1);
	
	return QCursor(pixels, mask);
}

void image_view::update_mouse_mode() {
	static QCursor circle_cursor = setup_circle_cursor();
	
	switch (mouse_mode::mode()) {
	case mouse_mode::none:
		setDragMode(QGraphicsView::NoDrag);
		viewport()->setCursor(Qt::ArrowCursor);
		break;
	case mouse_mode::pan:
		setDragMode(QGraphicsView::ScrollHandDrag);
		break;
	case mouse_mode::single_point:
	case mouse_mode::point:
		setDragMode(QGraphicsView::NoDrag);
		viewport()->setCursor(circle_cursor);
	}
}

void image_view::mouseMoveEvent(QMouseEvent *event) {
	if (mouse_mode::mode() == mouse_mode::point) {
		QPoint point = scene_point(event->pos());
		emit point_selected(point);
	}
	QGraphicsView::mouseMoveEvent(event);
}

void image_view::mousePressEvent(QMouseEvent *event) {
	if (event->button() == Qt::LeftButton) {
		QPoint point = scene_point(event->pos());
		
		if (mouse_mode::mode() == mouse_mode::single_point) {
			if (model->image_rect().contains(point)) {
				mouse_mode::set_global_mode(mouse_mode::none);
				emit point_selected(point);
			}
		} else if (mouse_mode::mode() == mouse_mode::point) {
			emit point_selected(point);
		}
	}
	QGraphicsView::mousePressEvent(event);
}

QPoint image_view::scene_point(const QPoint &pos) {
	QPointF pf = mapToScene(pos);
	// QPointF::toPoint() rounds to the nearest value, I'd rather always round down
	return QPoint((int)pf.x(), (int)pf.y());
}

void image_view::set_image(const QImage &image) {
	model->set_image(image);
	reset_zoom();
	scene()->setSceneRect(model->image_rect());
	
	// scale down the image if larger than the viewport, handle scrollbar bug
	QSize i_size = image.size();
	QSize v_size = viewport()->size();
	if (i_size.width() > v_size.width() || i_size.height() > v_size.height()) {
		setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
		setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
		
		fitInView(sceneRect(), Qt::KeepAspectRatio);
		
		setHorizontalScrollBarPolicy(Qt::ScrollBarAsNeeded);
		setVerticalScrollBarPolicy(Qt::ScrollBarAsNeeded);
	}
}

} // image
} // editor
