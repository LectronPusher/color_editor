#include "image_view.hpp"

#include <QToolButton>
#include <QFileDialog>
#include <QMessageBox>
#include <QTextStream>
#include <QDebug>

namespace editor{
namespace image {

image_view::image_view(QWidget *parent) : QGraphicsView(parent) {
	setScene(new QGraphicsScene(this));
	_base = new image_base(QImage());
	scene()->addItem(_base);
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
}

} // image
} // editor
