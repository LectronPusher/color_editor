#include "effect_types.hpp"

#include <QColor>
#include <QLabel>
#include <QLinearGradient>

namespace editor {
namespace color {
namespace effect_types {

// solid_color
solid_color::solid_color() : effect("Solid Color") {
	changeable_color = new color_label(this, Qt::red);
	auto label = new QLabel;
	label->setText("Color:");
	
	connect(changeable_color, &color_label::color_changed,
			this, &effect::altered);
	
	options->addWidget(label, 0, 0);
	options->addWidget(changeable_color, 0, 1);
	options->setRowStretch(1, 1);
}

QImage solid_color::create_mask(const QImage &, const QRect &rect) {
	QImage mask(rect.size(), QImage::Format_RGB32);
	mask.fill(changeable_color->color());
	return mask;
}
// end solid_color

// gradient
gradient::gradient() : effect("Gradient") {
	color_1 = new color_label(this, Qt::red);
	color_2 = new color_label(this, Qt::blue);
	auto label_1 = new QLabel;
	label_1->setText("Color 1:");
	auto label_2 = new QLabel;
	label_2->setText("Color 2:");
	orient_box = new QComboBox;
	orient_box->addItem("Horizontal", QVariant(true));
	orient_box->addItem("Vertical", QVariant(false));
	
	connect(color_1, &color_label::color_changed, this, &effect::altered);
	connect(color_2, &color_label::color_changed, this, &effect::altered);
	connect(orient_box, QOverload<int>::of(&QComboBox::currentIndexChanged),
			this, &effect::altered);
	
	options->addWidget(label_1, 0, 0);
	options->addWidget(color_1, 0, 1);
	options->addWidget(label_2, 1, 0);
	options->addWidget(color_2, 1, 1);
	options->addWidget(orient_box, 2, 0, 1, 2);
	options->setRowStretch(3, 1);
}

QImage gradient::create_mask(const QImage &, const QRect &rect) {
	QImage mask(rect.size(), QImage::Format_RGB32);
	QGradient gradient = create_gradient(rect.size());
	
	QPainter painter(&mask);
	painter.fillRect(QRect(QPoint(0, 0), rect.size()), gradient);
	
	return mask;
}

QGradient gradient::create_gradient(const QSize &size) {
	QLinearGradient grad;
	grad.setColorAt(0, color_1->color());
	grad.setColorAt(1, color_2->color());
	grad.setStart(0, 0);
	
	bool horizontal = orient_box->currentData().value<bool>();
	if (horizontal)
		grad.setFinalStop(size.width(), 0);
	else
		grad.setFinalStop(0, size.height());
	
	return grad;
}
// end gradient

} // effect_types
} // color
} // editor
