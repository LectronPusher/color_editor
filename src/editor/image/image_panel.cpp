#include "./image_panel.hpp"

#include <QVBoxLayout>
#include <QGraphicsView>
#include <QGraphicsScene>
#include <QLabel>
#include <QImage>

#include <QTextStream>

namespace editor{
namespace image {

QTextStream out2(stdout);

image_panel::image_panel(QWidget* parent) : QWidget(parent) {
	QImage image;
	bool loaded = image.load("../data/mantis300.jpg");
	if (!loaded)
		out2 << "loading failed" << Qt::endl;
	
	scene = new QGraphicsScene(this);
	scene->addItem(new image_base(image));
	QGraphicsView *view = new QGraphicsView(scene, this);
	
	QVBoxLayout *vbox = new QVBoxLayout;
	vbox->addWidget(view);
	setLayout(vbox);
}


} // image
} // editor
