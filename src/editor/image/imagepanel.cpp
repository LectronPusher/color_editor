#include "./imagepanel.hpp"

#include <QVBoxLayout>
#include <QLabel>
#include <QPixmap>

#include <QTextStream>

namespace editor{
namespace image {

QTextStream out2(stdout);

image_panel::image_panel(QWidget* parent) : QWidget(parent) {
	img_area = new image_area(this);
	img_area->load_image("../data/mantis300.jpg");
	
	QVBoxLayout *vbox = new QVBoxLayout;
	vbox->addWidget(img_area);
	setLayout(vbox);
}


} // image
} // editor
