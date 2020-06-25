#include "image_view.hpp"

#include <QToolButton>
#include <QFileDialog>
#include <QMessageBox>
#include <QDebug>

namespace editor{
namespace image {

//TODO scale on open (while loop?), fix center offset bug, fix /etc/samba save bug

image_view::image_view(QWidget *parent) : QGraphicsView(parent) {
	scene = new QGraphicsScene(this);
	scene->setBackgroundBrush(Qt::darkGray);
	setScene(scene);
}

void image_view::open_image() {
	save_request();
	QString filepath = QFileDialog::getOpenFileName(
		this, "Open Image", QDir().homePath(), "Image Files (*.png *.jpg *.bmp)"
	);
	QImage image(filepath);
	if (!image.isNull())
		set_image(image);
	else
		qDebug() << "loading failed";
}

void image_view::set_image(QImage image) {
	if (has_image) {
		scene->removeItem(base);
		delete base;
	}
	base = new image_base(image);
	scene->addItem(base);
	reset_zoom();
// 	while(scene->itemsBoundingRect().height > view->);
	has_image = true;
}

bool image_view::save_request() {
	if (has_image && image_modified) {
		QMessageBox::StandardButton ret;
		ret = QMessageBox::warning(
			this, "Unsaved Changes", 
			"The image has been modified.\nDo you want to save your changes?",
			QMessageBox::Save | QMessageBox::Discard | QMessageBox::Cancel
		);
		if (ret == QMessageBox::Save)
			save_as();
		else if (ret == QMessageBox::Cancel)
			return false;
	}
	return true;
}

void image_view::save_as() {
	QImage image = base->get_image();
	QString filepath = QFileDialog::getSaveFileName(
		this, "Save As", QDir().homePath() + "/untitled.png",
		"Image Files (*.png *.jpg *.bmp);;All Files (*)"
	);
	if (!filepath.isEmpty())
		image.save(filepath);
}

void image_view::zoom_in() {
	current_scale *= scale_by;
	scale(scale_by, scale_by);
}

void image_view::zoom_out() {
	current_scale /= scale_by;
	scale(1/scale_by, 1/scale_by);
}

void image_view::reset_zoom() {
	scale(1/current_scale, 1/current_scale);
	current_scale = 1;
}

} // image
} // editor
