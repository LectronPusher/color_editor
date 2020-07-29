#include "effect_types.hpp"

#include <QHBoxLayout>
#include <QToolButton>
#include <QLabel>
#include <QLinearGradient>
#include <QSlider>
#include <QSpinBox>
#include <QPainter>

namespace editor {
namespace color {
namespace effect_types {

// solid_color
solid_color::solid_color() : effect("Solid Color") {
	changeable_color = new color_label(Qt::red);
	
	connect(changeable_color, &color_label::color_changed,
			this, &effect::altered);
	
	auto hbox = new QHBoxLayout;
	hbox->addWidget(new QLabel("Color:"));
	hbox->addWidget(changeable_color);
	options->addLayout(hbox);
	options->addStretch(1);
}

image::mask solid_color::create_mask(const QImage &, const QRegion &region) {
	QImage img(region.boundingRect().size(), QImage::Format_ARGB32);
	img.fill(changeable_color->color());
	return {img, region};
}
// end solid_color


// gradient
gradient::gradient() : effect("Gradient") {
	color_1 = new color_label(Qt::cyan);
	color_2 = new color_label(Qt::yellow);
	orient_box = new QComboBox;
	orient_box->addItem("Horizontal", true);
	orient_box->addItem("Vertical", false);
	auto swap_b = new QToolButton;
	swap_b->setText("Swap colors");
	
	connect(color_1, &color_label::color_changed, this, &effect::altered);
	connect(color_2, &color_label::color_changed, this, &effect::altered);
	connect(orient_box, QOverload<int>::of(&QComboBox::currentIndexChanged),
			this, &effect::altered);
	connect(swap_b, &QToolButton::clicked, this, &gradient::swap_colors);
	
	auto hbox = new QHBoxLayout;
	hbox->addWidget(new QLabel("Color 1:"));
	hbox->addWidget(color_1);
	options->addLayout(hbox);
	hbox = new QHBoxLayout;
	hbox->addWidget(new QLabel("Color 2:"));
	hbox->addWidget(color_2);
	options->addLayout(hbox);
	options->addWidget(orient_box);
	options->addWidget(swap_b);
	options->setAlignment(swap_b, Qt::AlignCenter);
	options->addStretch(1);
}

image::mask gradient::create_mask(const QImage &, const QRegion &region) {
	QSize size = region.boundingRect().size();
	QImage img(size, QImage::Format_ARGB32);
	QGradient gradient = create_gradient(size);
	
	QPainter painter(&img);
	painter.fillRect(QRect(QPoint(0, 0), size), gradient);
	
	return {img, region};
}

void gradient::swap_colors() {
	QColor temp = color_1->color();
	color_1->set_color(color_2->color());
	color_2->set_color(temp);
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


// transparent
transparent::transparent() : effect("Transparent Color") {
	trans_label = new color_label(Qt::red);
	trans_label->enable_transparency();
	auto slider = new QSlider(Qt::Horizontal);
	slider->setMinimum(0);
	slider->setMaximum(255);
	auto spinbox = new QSpinBox;
	spinbox->setRange(0, 255);
	override_box = new QComboBox;
	override_box->addItem("Layer Over", false);
	override_box->addItem("Replace", true);
	
	connect(trans_label, &color_label::color_changed, this, &effect::altered);
	connect(slider, &QSlider::valueChanged,
			this, &transparent::set_label_transparency);
	connect(slider, &QSlider::valueChanged, spinbox, &QSpinBox::setValue);
	connect(spinbox, QOverload<int>::of(&QSpinBox::valueChanged),
			slider, &QSlider::setValue);
	connect(override_box, QOverload<int>::of(&QComboBox::currentIndexChanged),
			this, &effect::altered);
	
	slider->setValue(80);
	
	auto hbox = new QHBoxLayout;
	hbox->addWidget(new QLabel("Color:"));
	hbox->addWidget(trans_label);
	options->addLayout(hbox);
	hbox = new QHBoxLayout;
	hbox->addWidget(new QLabel("Opacity:"));
	hbox->addWidget(spinbox);
	spinbox->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
	options->addLayout(hbox);
	options->addWidget(slider);
	options->addWidget(override_box);
	options->addStretch(1);
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
// end transparent


// pixellate
pixellate::pixellate() : effect("Pixellate") {
	pixel_size = new QSpinBox;
	pixel_size->setRange(1, 5000);
	pixel_size->setValue(5);
	
	connect(pixel_size, QOverload<int>::of(&QSpinBox::valueChanged),
			this, &effect::altered);
	
	auto hbox = new QHBoxLayout;
	hbox->addWidget(new QLabel("Pixel Size:"));
	hbox->addWidget(pixel_size);
	pixel_size->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
	options->addLayout(hbox);
	options->addStretch(1);
}

image::mask pixellate::create_mask(const QImage &image, const QRegion &region) {
	const int size = pixel_size->value();
	if (size == 1)
		return {image, region};
	
	QRect rect = region.boundingRect();
	QImage pixellated = image.copy(rect);
	QPainter painter(&pixellated);
	int start = (size % 2 == 0) ? (size - 1) /2 : size / 2;
	for (int row = rect.top(); row < image.width(); row += size) {
		for (int col = rect.left(); col < image.height(); col += size) {
			QPoint point(row, col);
			painter.fillRect(create_rect(point), pixellated.pixelColor(point));
		}
	}
// 	if (image.width() % size != 0) {
// 		painter.fillRect(QRect(QPoint(), QSize()), image.pixelColor(point));
// 	}
	
	return {pixellated, region};
}

QRect pixellate::create_rect(const QPoint &point) {
	int s = pixel_size->value();
	int b = (s % 2 == 0) ? (s - 1) /2 : s / 2;
	return QRect(point - QPoint(b, b), QSize(s, s));
}
// end pixellate

} // effect_types
} // color
} // editor
