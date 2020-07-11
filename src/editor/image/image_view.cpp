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
	setBackgroundBrush(Qt::darkGray);
}

QSize image_view::minimumSizeHint() const {
	return QSize(304, 304);
}

const QImage image_view::get_image() const {
	if (has_image)
		return base->get_image();
	else
		return QImage();
}

void image_view::set_mask(const QImage &new_mask, const QRegion &region) {
	base->set_mask(new_mask, region);
	scene()->update(region.boundingRect());
	image_modified = true;
}

void image_view::apply_mask() {
	base->apply_mask();
}

void image_view::open_image(QString filepath) {
	if (maybe_save()) {
		if (filepath.isEmpty()) {
			filepath = QFileDialog::getOpenFileName(
				this,
				"Open Image",
				old_file.dir().path(),
				"Image Files (*.png *.jpg *.bmp)"
			);
		}
		QImage image;
		if (!filepath.isEmpty()) {
			old_file = QFileInfo(filepath);
			image = QImage(filepath);
			if (!image.isNull())
				set_image(image);
			else
				QTextStream(stdout) << "loading failed:" << filepath << Qt::endl;
		}
	}
}

void image_view::save_as() {
	if (has_image) {
		QString filepath = QFileDialog::getSaveFileName(
			this,
			"Save As",
			old_file.filePath(),
			"Image Files (*.png *.jpg *.bmp);;All Files (*)"
		);
		if (!filepath.isEmpty()) {
			QImage image = base->apply_mask();
			image_modified = false;
			image.save(filepath);
			old_file = QFileInfo(filepath);
		}
	}
}

bool image_view::maybe_save() {
	if (has_image && image_modified) {
		auto dialog_ans = QMessageBox::warning(
			this,
			"Unsaved Changes", 
			"The image has been modified.\nDo you want to save your changes?",
			QMessageBox::Save | QMessageBox::Discard | QMessageBox::Cancel
		);
		if (dialog_ans == QMessageBox::Save)
			save_as();
		else if (dialog_ans == QMessageBox::Cancel)
			return false;
	}
	return true;
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
	// delete the old image if there was one
	if (has_image) {
		scene()->removeItem(base);
		delete base;
	}
	
	// add the new image and update variables
	base = new image_base(image);
	has_image = true;
	image_modified = false;
	scene()->addItem(base);
	
	// reset the scene rectangle and zoom
	reset_zoom();
	scene()->setSceneRect(base->boundingRect());
	updateSceneRect(base->boundingRect());
	
	// resize the image if it's larger than the viewport, handles scrollbars
	if (image.size().boundedTo(viewport()->size()) != image.size()) {
		setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
		setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
		
		fitInView(sceneRect(), Qt::KeepAspectRatio);
		
		setHorizontalScrollBarPolicy(Qt::ScrollBarAsNeeded);
		setVerticalScrollBarPolicy(Qt::ScrollBarAsNeeded);
	}
}

} // image
} // editor
