#include "color_label.hpp"

#include <QPainter>

using namespace editor::color;

color_label::color_label(QColor starting_color, bool allow_transparency,
						 bool allow_interaction, QWidget *parent)
: QFrame(parent), fill_color(starting_color),
can_be_transparent(allow_transparency), is_interactive(allow_interaction){
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
		if (!can_be_transparent)
			fill_color.setAlpha(255);
		QWidget::update(); // do a redraw
		emit color_changed(fill_color);
	}
}

void color_label::set_alpha(int value) {
	if (can_be_transparent && value != fill_color.alpha()) {
		fill_color.setAlpha(value);
		emit color_changed(fill_color);
	}
}

void color_label::paintEvent(QPaintEvent *event) {
	QPainter painter(this);
	painter.fillRect(QRect(QPoint(0, 0), sizeHint()), fill_color);
	QFrame::paintEvent(event);
}

void color_label::hideEvent(QHideEvent *event) {
	if (dialog)
		dialog->QDialog::done(1);
	QFrame::hideEvent(event);
}

void color_label::mousePressEvent(QMouseEvent *event) {
	if (is_interactive && dialog == nullptr)
		open_dialog();
	QFrame::mousePressEvent(event);
}

void color_label::open_dialog() {
	dialog = new QColorDialog(fill_color, this);
	dialog->setOption(QColorDialog::DontUseNativeDialog);
	dialog->setOption(QColorDialog::NoButtons);
	if (can_be_transparent)
		dialog->setOption(QColorDialog::ShowAlphaChannel);
	
	connect(dialog, &QColorDialog::currentColorChanged,
			this, &color_label::set_color);
	connect(this, &color_label::color_changed,
			dialog, &QColorDialog::setCurrentColor);
	// connecting to finished works, but connecting to destroyed doesn't, weird
	connect(dialog, &QColorDialog::finished, this, [=](){dialog = nullptr;});
	
	dialog->show();
}
