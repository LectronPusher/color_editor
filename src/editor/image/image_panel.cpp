#include "./image_panel.hpp"

#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QGraphicsView>
#include <QLabel>
#include <QImage>
#include <QToolButton>

#include <QTextStream>

namespace editor{
namespace image {

QTextStream out2(stdout);

image_panel::image_panel(QWidget* parent) : QWidget(parent) {
	// load the image
	QImage image;
	bool loaded = image.load("../data/mantis300.jpg");
	if (!loaded)
		out2 << "loading failed" << Qt::endl;
	
	scene = new QGraphicsScene(this);
	scene->addItem(new image_base(image));
	QGraphicsView *view = new QGraphicsView(scene, this);
	
	// create buttons to control the image
	QToolButton *zoom_in = new QToolButton;
	QToolButton *zoom_out = new QToolButton;
	zoom_in->setText("Zoom In");
	zoom_out->setText("Zoom Out");
	connect(zoom_in, &QToolButton::clicked,
			view, [view](){ view->scale(1.2, 1.2); });
	connect(zoom_out, &QToolButton::clicked,
			view, [view](){ view->scale(1/1.2, 1/1.2); });
	// add the buttons to an hbox above the image
	QHBoxLayout *button_holder = new QHBoxLayout;
	button_holder->addWidget(zoom_in);
	button_holder->addWidget(zoom_out);
	button_holder->addStretch();
	
	// add everything to a central layout
	QVBoxLayout *vbox = new QVBoxLayout;
	vbox->addLayout(button_holder);
	vbox->addWidget(view);
	setLayout(vbox);
}


} // image
} // editor
