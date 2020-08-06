#include "effect_types.hpp"

#include <QHBoxLayout>
#include <QToolButton>
#include <QLabel>
#include <QLinearGradient>
#include <QSlider>
#include <QSpinBox>
#include <QPainter>

using namespace editor::color::effect_types;

// single_color
single_color::single_color() : effect("Single Color") {
	stored_color = new color_label(Qt::red);
	stored_color->enable_transparency();
	auto slider = new QSlider(Qt::Horizontal);
	slider->setMinimum(0);
	slider->setMaximum(255);
	auto spinbox = new QSpinBox;
	spinbox->setRange(0, 255);
	paint_mode_box = new QComboBox;
	paint_mode_box->addItem("Layer Over", editor_model::over);
	paint_mode_box->addItem("Replace", editor_model::replace);
	
	connect(stored_color, &color_label::color_changed, this, &effect::altered);
	connect(slider, &QSlider::valueChanged,
			this, &single_color::set_label_transparency);
	connect(slider, &QSlider::valueChanged, spinbox, &QSpinBox::setValue);
	connect(spinbox, QOverload<int>::of(&QSpinBox::valueChanged),
			slider, &QSlider::setValue);
	connect(paint_mode_box, QOverload<int>::of(&QComboBox::currentIndexChanged),
			this, &effect::altered);
	
	slider->setValue(255);
	
	auto hbox = new QHBoxLayout;
	hbox->addWidget(new QLabel("Color:"));
	hbox->addWidget(stored_color);
	options->addLayout(hbox);
	hbox = new QHBoxLayout;
	hbox->addWidget(new QLabel("Opacity:"));
	hbox->addWidget(spinbox);
	spinbox->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
	options->addLayout(hbox);
	options->addWidget(slider);
	options->addWidget(paint_mode_box);
	options->addStretch(1);
}

void single_color::set_label_transparency(int value) {
	QColor color = stored_color->color();
	color.setAlpha(value);
	stored_color->set_color(color);
}

QImage single_color::create_mask(editor_model *model) {
	QImage img(model->region_rect().size(), QImage::Format_ARGB32);
	img.fill(stored_color->color());
	return img;
}

editor::editor_model::painting_mode single_color::paint_mode() {
	return paint_mode_box->currentData().value<editor_model::painting_mode>();
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

QImage gradient::create_mask(editor_model *model) {
	QSize size = model->region_rect().size();
	QImage img(size, QImage::Format_ARGB32);
	QGradient gradient = create_gradient(size);
	
	QPainter painter(&img);
	painter.fillRect(QRect(QPoint(0, 0), size), gradient);
	
	return img;
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

QImage pixellate::create_mask(editor_model *model) {
	const QRect &rect = model->region_rect();
	if (rect.isEmpty())
		return QImage();
	QImage source = model->source_image().copy(rect);
	if (pixel_size->value() == 1)
		return source;
	
	QImage pixellated = source;
	QPainter painter(&pixellated);
	painter.setCompositionMode(QPainter::CompositionMode_Source);
	pixellate_image(&painter, source);
	return pixellated;
}

static int under_half(int i) {
	return (i % 2 == 0) ? (i - 1) / 2 : i / 2;
}

void pixellate::pixellate_image(QPainter *painter, const QImage &source) {
	int size = pixel_size->value();
	int h_s = under_half(size);
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
	int h = under_half(s);
	return {point - QPoint(h, h), QSize(s, s)};
}
// end pixellate
