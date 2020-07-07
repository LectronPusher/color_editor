#include "color_label.hpp"

#include <QVBoxLayout>
#include <QColorDialog>

namespace editor {
namespace color {

color_label::color_label(QWidget *parent, QColor starting_color)
: QWidget(parent), fill_color(starting_color) {
	QWidget::resize(25, 25);
	setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
}

QColor color_label::color() const {
	return fill_color;
}

void color_label::set_color(QColor new_color) {
	fill_color = new_color;
	update();
}

QSize color_label::sizeHint() const {
	return QWidget::size();
}

void color_label::set_interactive(bool interactive) {
	is_interactive = interactive;
}

void color_label::paintEvent(QPaintEvent *) {
	QPainter painter(this);
	painter.fillRect(QRect(QPoint(0, 0), QWidget::size()), fill_color);
}

void color_label::mousePressEvent(QMouseEvent *) {
	if(is_interactive)
		open_dialog();
}

void color_label::open_dialog() {
	
}




} // color
} // editor
