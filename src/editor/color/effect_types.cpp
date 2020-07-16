#include "effect_types.hpp"

#include <QColor>
#include <QLabel>
#include <QLinearGradient>
#include <QSlider>
#include <QSpinBox>

namespace editor {
namespace color {
namespace effect_types {

// solid_color
solid_color::solid_color() : effect("Solid Color") {
	changeable_color = new color_label(this, Qt::red);
	
	connect(changeable_color, &color_label::color_changed,
			this, &effect::altered);
	
	options->addWidget(new QLabel("Color:"), 0, 0);
	options->addWidget(changeable_color, 0, 1);
	options->setRowStretch(1, 1);
}

image::mask solid_color::create_mask(const QImage &, const QRegion &region) {
	QImage img(region.boundingRect().size(), QImage::Format_ARGB32);
	img.fill(changeable_color->color());
	return {img, region};
}
// end solid_color

// gradient
gradient::gradient() : effect("Gradient") {
	color_1 = new color_label(this, Qt::cyan);
	color_2 = new color_label(this, Qt::yellow);
	orient_box = new QComboBox;
	orient_box->addItem("Horizontal", QVariant(true));
	orient_box->addItem("Vertical", QVariant(false));
	
	connect(color_1, &color_label::color_changed, this, &effect::altered);
	connect(color_2, &color_label::color_changed, this, &effect::altered);
	connect(orient_box, QOverload<int>::of(&QComboBox::currentIndexChanged),
			this, &effect::altered);
	
	options->addWidget(new QLabel("Color 1:"), 0, 0);
	options->addWidget(color_1, 0, 1);
	options->addWidget(new QLabel("Color 2:"), 1, 0);
	options->addWidget(color_2, 1, 1);
	options->addWidget(orient_box, 2, 0, 1, 2);
	options->setRowStretch(3, 1);
}

image::mask gradient::create_mask(const QImage &, const QRegion &region) {
	QSize size = region.boundingRect().size();
	QImage img(size, QImage::Format_ARGB32);
	QGradient gradient = create_gradient(size);
	
	QPainter painter(&img);
	painter.fillRect(QRect(QPoint(0, 0), size), gradient);
	
	return {img, region};
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

// transparent_override
transparent::transparent() : effect("Transparent Color") {
	trans_label = new color_label(this, Qt::magenta);
	trans_label->enable_transparency();
	auto slider = new QSlider(Qt::Horizontal);
	slider->setMinimum(0);
	slider->setMaximum(255);
	auto spinbox = new QSpinBox;
	spinbox->setRange(0, 255);
	override_box = new QComboBox;
	override_box->addItem("Replace", QVariant(true));
	override_box->addItem("Layer Over", QVariant(false));
	
	connect(trans_label, &color_label::color_changed, this, &effect::altered);
	connect(slider, &QSlider::valueChanged,
			this, &transparent::set_label_transparency);
	connect(slider, &QSlider::valueChanged, spinbox, &QSpinBox::setValue);
	connect(spinbox, QOverload<int>::of(&QSpinBox::valueChanged),
			slider, &QSlider::setValue);
	connect(override_box, QOverload<int>::of(&QComboBox::currentIndexChanged),
			this, &effect::altered);
	
	slider->setValue(1);
	slider->setValue(0);
	
	options->addWidget(new QLabel("Color:"), 0, 0);
	options->addWidget(trans_label, 0, 1);
	options->setAlignment(trans_label, Qt::AlignRight);
	options->addWidget(new QLabel("Opacity:"), 1, 0);
	options->addWidget(spinbox, 1, 1);
	spinbox->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
	options->addWidget(slider, 2, 0, 1, 2);
	options->addWidget(override_box, 3, 0, 1, 2);
	options->setRowStretch(4, 1);
}

void transparent::set_label_transparency(int value) {
	QColor color = trans_label->color();
	color.setAlpha(value);
	trans_label->set_color(color);
}

image::mask transparent::create_mask(const QImage &, const QRegion &region) {
	QImage img(region.boundingRect().size(), QImage::Format_ARGB32);
	img.fill(trans_label->color());
	bool paint_over = override_box->currentData().value<bool>();
	return {img, region, paint_over};
}
// end transparent_override

} // effect_types
} // color
} // editor
