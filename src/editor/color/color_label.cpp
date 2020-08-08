#include "color_label.hpp"

#include <QVBoxLayout>
#include <QColorDialog>

using namespace editor::color;

color_label::color_label(QColor starting_color, QWidget *parent)
: QFrame(parent), fill_color(starting_color) {
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

void color_label::disable_interaction() {
	is_interactive = false;
}

void color_label::enable_transparency() {
	can_be_transparent = true;
}

void color_label::set_color(const QColor &new_color) {
	if (new_color.isValid() && fill_color != new_color) {
		fill_color = new_color;
		if (!can_be_transparent)
			fill_color.setAlpha(255);
		QWidget::update();
		emit color_changed(fill_color);
	}
}

void color_label::set_alpha(int value) {
	if (can_be_transparent)
		fill_color.setAlpha(value);
}

void color_label::paintEvent(QPaintEvent *event) {
	QPainter painter(this);
	painter.fillRect(QRect(QPoint(0, 0), sizeHint()), fill_color);
	QFrame::paintEvent(event);
}

void color_label::mousePressEvent(QMouseEvent *) {
	if (is_interactive)
		open_dialog();
}

void color_label::hideEvent(QHideEvent *event) {
	emit hidden(0);
	QFrame::hideEvent(event);
}

void color_label::open_dialog() {
	auto dialog = new QColorDialog(fill_color, this);
	dialog->setOption(QColorDialog::DontUseNativeDialog);
	dialog->setOption(QColorDialog::NoButtons);
	if (can_be_transparent)
		dialog->setOption(QColorDialog::ShowAlphaChannel);
	
	connect(dialog, &QColorDialog::currentColorChanged,
			this, &color_label::set_color);
	connect(this, &color_label::color_changed,
			dialog, [dialog](const QColor &c){ dialog->setCurrentColor(c); });
	connect(this, &color_label::hidden, dialog, &QDialog::done);
	
	dialog->show();
}
