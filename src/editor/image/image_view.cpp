#include "image_view.hpp"

#include <QToolButton>
#include <QFileDialog>
#include <QMessageBox>
#include <QDebug>
#include <QTextStream>
#include <QDir>

namespace editor{
namespace image {

//TODO figure out /etc/samba save bug, probably not a me issue, but still possible

image_view::image_view(QWidget *parent) : QGraphicsView(parent) {
	image_scene = new QGraphicsScene(this);
	setScene(image_scene);
	setBackgroundBrush(Qt::darkGray);
}

QSize image_view::minimumSizeHint() const {
	return QSize(300, 300);
}

void image_view::open_image(QString filepath) {
	if (maybe_save()) {
		if (filepath.isEmpty()) {
			filepath = QFileDialog::getOpenFileName(
				this,
				"Open Image",
				"home/ian/all/coding/c++/color_editor/data/",
				"Image Files (*.png *.jpg *.bmp)"
			);
		}
		QImage image;
		if (!filepath.isEmpty()) {
			image = QImage(filepath);
			if (!image.isNull())
				set_image(image);
			else
				QTextStream(stdout) << "loading failed:" << filepath << Qt::endl;
		}
	}
}

void image_view::set_image(QImage image) {
	if (has_image) {
		image_scene->removeItem(base);
		delete base;
	}
	base = new image_base(image);
	has_image = true;
	image_scene->addItem(base);
	image_scene->setSceneRect(base->boundingRect());
	updateSceneRect(base->boundingRect());
	fitInView(base, Qt::KeepAspectRatio);
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

void image_view::save_as() {
	if (has_image) {
		QString filepath = QFileDialog::getSaveFileName(
			this,
			"Save As",
			"home/ian/all/coding/c++/color_editor/data/untitled.png",
			"Image Files (*.png *.jpg *.bmp);;All Files (*)"
		);
		if (!filepath.isEmpty())
			base->get_image().save(filepath);
	}
}

void image_view::zoom_in() {
	scale(scale_by, scale_by);
}

void image_view::zoom_out() {
	scale(1/scale_by, 1/scale_by);
}

QImage image_view::get_image() {
	if (has_image)
		return base->get_image();
	else
		return QImage();
}

} // image
} // editor
