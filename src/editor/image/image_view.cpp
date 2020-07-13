#include "image_view.hpp"

#include <QToolButton>
#include <QFileDialog>
#include <QMessageBox>
#include <QTextStream>
#include <QCursor>
#include <QBitmap>
#include <QPainter>
#include <QDebug>

namespace editor{
namespace image {

image_view::image_view(QWidget *parent) : QGraphicsView(parent) {
	setScene(new QGraphicsScene(this));
	_base = new image_base(QImage());
	scene()->addItem(_base);
	
	set_mouse_mode(pan);
	setBackgroundBrush(Qt::darkGray);
}

image_base *image_view::base() {
	return _base;
}

bool image_view::modifications_resolved() {
	if (_base->has_image() && _base->is_modified()) {
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

void image_view::set_mouse_mode(image_view::mouse_mode new_mode) {
	
	switch (new_mode) {
	case pan:
		setDragMode(QGraphicsView::ScrollHandDrag);
		break;
	case color:
		setDragMode(QGraphicsView::NoDrag);
		viewport()->setCursor(Qt::CrossCursor);
		break;
	case point:
		static QCursor circle_cursor = setup_circle_cursor();
		setDragMode(QGraphicsView::NoDrag);
		viewport()->setCursor(circle_cursor);
		
	}
	mode = new_mode;
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
	if (_base->has_image()) {
		QString filepath = QFileDialog::getSaveFileName(
			this,
			"Save As",
			old_file.filePath(),
			"Image Files (*.png *.jpg *.bmp);;All Files (*)"
		);
		if (!filepath.isEmpty()) {
			QImage image = _base->apply_mask();
			image.save(filepath);
			set_image(image);
			old_file = QFileInfo(filepath);
		}
	}
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

void image_view::mouseMoveEvent(QMouseEvent *event) {
	if (mode == point) {
		QPoint point = mapToScene(event->pos()).toPoint();
		if (_base->boundingRect().contains(point))
			emit point_selected(point);
	}
	QGraphicsView::mouseMoveEvent(event);
}

void image_view::mousePressEvent(QMouseEvent * event) {
	if (mode == color && event->button() == Qt::LeftButton) {
		QPoint point = mapToScene(event->pos()).toPoint();
		if (_base->boundingRect().contains(point))
			emit color_selected(_base->image().pixelColor(point));
	}
	QGraphicsView::mousePressEvent(event);
}

void image_view::set_image(const QImage &image) {
	// destroy old, create new
	scene()->removeItem(_base);
	delete _base;
	_base = new image_base(image);
	scene()->addItem(_base);
	
	// update visuals
	reset_zoom();
	scene()->setSceneRect(_base->boundingRect());
	
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
	
	emit base_image_changed(image);
}

} // image
} // editor
