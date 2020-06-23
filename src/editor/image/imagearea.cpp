#include "./imagearea.hpp"

#include <QPainter>
#include <QTextStream>

namespace editor{
namespace image {

QTextStream out3(stdout);

image_area::image_area(QWidget* parent) : QWidget(parent) {
	
}

void image_area::load_image(QString filename) {
	out3 << "loading" << Qt::endl;
	if(img.load(filename))
		out3 << "loaded" << Qt::endl;
	else
		out3 << "failed" << Qt::endl;
		
}

QSize image_area::sizeHint() const {
	out3 << "sizehint" << Qt::endl;
	return img.size();
}

void image_area::paintEvent(QPaintEvent *) {
// 	out3 << "paitevent" << Qt::endl;
	QPainter painter(this);
	painter.drawImage(img.rect(), img);
}



} // image
} // editor
 
