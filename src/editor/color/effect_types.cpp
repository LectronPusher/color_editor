#include "effect_types.hpp"

#include <QHBoxLayout>
#include <QToolButton>
#include <QLabel>
#include <QLinearGradient>
#include <QSlider>
#include <QSpinBox>
#include <QPainter>

Q_DECLARE_METATYPE(editor::painting_mode::mode);

using namespace editor::color::effect_types;

// single_color
single_color::single_color() : effect("Single Color") {
	stored_color = new color_label(Qt::red);
	stored_color->enable_transparency();
	auto spinbox = new QSpinBox;
	spinbox->setRange(0, 255);
	auto slider = new QSlider(Qt::Horizontal);
	slider->setMinimum(0);
	slider->setMaximum(255);
	auto mode_box = new QComboBox;
	
	connect(stored_color, &color_label::color_changed, this, &effect::altered);
	connect(stored_color, &color_label::color_changed,
			this, [slider](const QColor &c){ slider->setValue(c.alpha()); });
	connect(slider, &QSlider::valueChanged, stored_color, &color_label::set_alpha);
	connect(slider, &QSlider::valueChanged, spinbox, &QSpinBox::setValue);
	auto value_changed = QOverload<int>::of(&QSpinBox::valueChanged);
	connect(spinbox, value_changed, slider, &QSlider::setValue);
	auto index_changed = QOverload<int>::of(&QComboBox::currentIndexChanged);
	connect(mode_box, index_changed, this, [this, mode_box](){
		set_mode(mode_box->currentData().value<painting_mode::mode>());
	});
	
	// add the values after the connections to update others
	mode_box->addItem("Overlay", painting_mode::over);
	mode_box->addItem("Replace", painting_mode::replace);
	slider->setValue(255);
	
	auto hbox = new QHBoxLayout;
	hbox->addWidget(new QLabel("Color:"));
	hbox->addWidget(stored_color);
	options->addLayout(hbox);
	hbox = new QHBoxLayout;
	hbox->addWidget(new QLabel("Opacity:"));
	hbox->addStretch(1);
	hbox->addWidget(spinbox);
	options->addLayout(hbox);
	options->addWidget(slider);
	options->addWidget(mode_box);
	options->addStretch(1);
}

QImage single_color::create_effect(const QImage &image, const QRect &) {
	QImage effect(image.size(), QImage::Format_ARGB32);
	effect.fill(stored_color->color());
	return effect;
}
// end single_color


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
	
	options->addWidget(orient_box);
	auto hbox = new QHBoxLayout;
	hbox->addWidget(new QLabel("Color 1:"));
	hbox->addWidget(color_1);
	options->addLayout(hbox);
	hbox = new QHBoxLayout;
	hbox->addWidget(new QLabel("Color 2:"));
	hbox->addWidget(color_2);
	options->addLayout(hbox);
	options->addWidget(swap_b);
	options->setAlignment(swap_b, Qt::AlignCenter);
	options->addStretch(1);
}

QImage gradient::create_effect(const QImage &image, const QRect &boundary) {
	QImage effect(image.size(), QImage::Format_ARGB32);
	QPainter painter(&effect);
	painter.fillRect(image.rect(), create_gradient(boundary));
	return effect;
}

void gradient::swap_colors() {
	QColor temp = color_1->color();
	color_1->set_color(color_2->color());
	color_2->set_color(temp);
}

QGradient gradient::create_gradient(const QRect &rect) {
	QLinearGradient grad;
	grad.setColorAt(0, color_1->color());
	grad.setColorAt(1, color_2->color());
	grad.setStart(rect.left(), rect.top());
	
	bool horizontal = orient_box->currentData().value<bool>();
	if (horizontal)
		grad.setFinalStop(rect.right(), rect.top());
	else
		grad.setFinalStop(rect.left(), rect.bottom());
	
	return grad;
}
// end gradient


// pixellate
pixellate::pixellate() : effect("Pixellate") {
	pixel_size = new QSpinBox;
	pixel_size->setRange(1, 5000);
	pixel_size->setValue(5);
	
	connect(pixel_size, QOverload<int>::of(&QSpinBox::valueChanged),
			this, &effect::altered);
	
	auto hbox = new QHBoxLayout;
	hbox->addWidget(new QLabel("Pixel Size:"));
	hbox->addStretch(1);
	hbox->addWidget(pixel_size);
	options->addLayout(hbox);
	options->addStretch(1);
}

QImage pixellate::create_effect(const QImage &image, const QRect &) {
	if (pixel_size->value() == 1)
		return image;
	
	QImage effect(image.size(), QImage::Format_ARGB32);
	QPainter painter(&effect);
	painter.setCompositionMode(QPainter::CompositionMode_Source);
	pixellate_image(&painter, image);
	return effect;
}

void pixellate::pixellate_image(QPainter *painter, const QImage &source) {
	int size = pixel_size->value();
	int h_s = (size - 1) / 2; // half of size (matches create_rect)
	int width = source.width();
	int height = source.height();
	
	// fill main portion of image
	for (int row = h_s; row < width; row += size) {
		for (int col = h_s; col < height; col += size) {
			QPoint point(row, col);
			painter->fillRect(create_rect(point), source.pixelColor(point));
		}
	}
	// fill remaining edges and bottom right corner
	int width_mod = (width % size <= h_s) ? width % size : 0;
	int height_mod = (height % size <= h_s) ? height % size : 0;
	QRect right(width - width_mod, 0, width_mod, height);
	QRect bottom(0, height - height_mod, width, height_mod);
	// fill right
	for (int col = h_s; col < height; col += size) {
		QPoint point(width - 1, col);
		painter->fillRect(right & create_rect(point), source.pixelColor(point));
	}
	// fill bottom
	for (int row = h_s; row < width; row += size) {
		QPoint point(row, height - 1);
		painter->fillRect(bottom & create_rect(point), source.pixelColor(point));
	}
	// fill corner
	painter->fillRect(right & bottom, source.pixelColor(width - 1, height - 1));
}

QRect pixellate::create_rect(const QPoint &point) {
	int s = pixel_size->value();
	// make point center if odd, top left of central square if even
	int h_s = (s - 1) / 2;
	return {point - QPoint(h_s, h_s), QSize(s, s)};
}
// end pixellate
