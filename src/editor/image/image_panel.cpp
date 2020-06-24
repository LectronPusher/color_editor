#include "./image_panel.hpp"

#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QToolButton>
#include <QFileDialog>
#include <QMessageBox>
#include <QDebug>

namespace editor{
namespace image {

image_panel::image_panel(QWidget *parent) : QWidget(parent) {
	// place the image in the scene and the scene in the view
	scene = new QGraphicsScene(this);
	scene->setBackgroundBrush(Qt::darkGray);
	view = new QGraphicsView(scene, this);
	open_image(true);
	
	// create buttons to control the image
	QToolButton *open_b = new QToolButton(this);
	open_b->setText("Open");
	QToolButton *save_as_b = new QToolButton(this);
	save_as_b->setText("Save As");
	QToolButton *zoom_in_b = new QToolButton(this);
	zoom_in_b->setText("Zoom In");
	QToolButton *zoom_out_b = new QToolButton(this);
	zoom_out_b->setText("Zoom Out");
	// connect the buttons to their functions
	connect(open_b, &QToolButton::clicked, this, [this](){ this->open_image(); });
	connect(save_as_b, &QToolButton::clicked, this, [this](){ this->save_as(); });
	connect(zoom_in_b, &QToolButton::clicked, this, &image_panel::zoom_in);
	connect(zoom_out_b, &QToolButton::clicked, this, &image_panel::zoom_out);
	// add the buttons to an hbox above the image
	QHBoxLayout *button_holder = new QHBoxLayout;
	button_holder->addWidget(open_b);
	button_holder->addWidget(save_as_b);
	button_holder->addWidget(zoom_in_b);
	button_holder->addWidget(zoom_out_b);
	button_holder->addStretch();
	
	// add everything to a central layout
	QVBoxLayout *vbox = new QVBoxLayout;
	vbox->addLayout(button_holder);
	vbox->addWidget(view);
	setLayout(vbox);
}

void image_panel::open_image(bool oof) {
	QString filepath;
	if (!oof) {
		filepath = QFileDialog::getOpenFileName(
			this, "Open Image", QDir().homePath(),
			"Image Files (*.png *.jpg *.bmp)"
		);
	} else {
		filepath = "../data/mantis300.jpg";
	}
	
	QImage image(filepath);
	if (!image.isNull())
		set_image(image);
	else
		qDebug() << "loading failed";
}

void image_panel::set_image(QImage image) {
	if (has_image) {
		scene->removeItem(base);
		delete base;
	}
	base = new image_base(image);
	scene->addItem(base);
	scene->update(scene->itemsBoundingRect());
	has_image = true;
}

bool image_panel::save_request() {
	if (has_image) {
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

void image_panel::save_as() {
	QImage image = base->get_image();
	QString filepath = QFileDialog::getSaveFileName(
		this, "Save As", QDir().homePath() + "/untitled.png",
		"Image Files (*.png *.jpg *.bmp);;All Files (*)"
	);
	if (!filepath.isEmpty())
		image.save(filepath);
}

void image_panel::zoom_in() {
	view->scale(1.2, 1.2);
}

void image_panel::zoom_out() {
	view->scale(1/1.2, 1/1.2);
}

} // image
} // editor

//TODO scale on open_image() (while loop?), fix /etc/samba save bug, fix center offset bug
