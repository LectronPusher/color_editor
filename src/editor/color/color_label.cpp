#include "color_label.hpp"

#include <QVBoxLayout>
#include <QColorDialog>

namespace editor {
namespace color {

color_label::color_label(QWidget *parent, QColor starting_color, bool interactive)
: QFrame(parent), is_interactive(interactive), fill_color(starting_color) {
	QWidget::resize(25, 25);
	setFrameStyle(QFrame::Panel | QFrame::Sunken);
	setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
}

QSize color_label::sizeHint() const {
	return QWidget::size();
}

QColor color_label::color() const {
	return fill_color;
}

void color_label::set_color(const QColor &new_color) {
	if (new_color.isValid() && fill_color != new_color) {
		fill_color = new_color;
		QWidget::update();
		emit color_changed(new_color);
	}
}

void color_label::paintEvent(QPaintEvent *event) {
	QPainter painter(this);
	painter.fillRect(QRect(QPoint(0, 0), sizeHint()), fill_color);
	QFrame::paintEvent(event);
}

void color_label::mousePressEvent(QMouseEvent *) {
	if(is_interactive)
		open_dialog();
}

void color_label::open_dialog() {
	auto dialog = new QColorDialog(fill_color, this);
	dialog->setOption(QColorDialog::DontUseNativeDialog);
	dialog->setOption(QColorDialog::NoButtons);
	
	connect(dialog, &QColorDialog::currentColorChanged,
			this, &color_label::set_color);
	
	dialog->show();
}

} // color
} // editor
