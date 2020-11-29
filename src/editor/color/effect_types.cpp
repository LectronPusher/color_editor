#include "effect_types.hpp"

#include <QHBoxLayout>
#include <QToolButton>
#include <QLabel>
#include <QSlider>

using namespace editor::color;

// single_color
single_color_widget::single_color_widget() : effect_widget("Single Color") {
	stored_color = new color_label(Qt::red, true);
	paint_mode_box = new QComboBox;
	auto spinbox = new QSpinBox;
	spinbox->setRange(0, 255);
	auto slider = new QSlider(Qt::Horizontal);
	slider->setRange(0, 255);
	
	connect(stored_color, &color_label::color_changed,
			this, [=](){ emit altered(get_state()); });
	connect(stored_color, &color_label::color_changed,
			this, [slider](const QColor &c){ slider->setValue(c.alpha()); });
	connect(slider, &QSlider::valueChanged,
			stored_color, &color_label::set_alpha);
	connect(paint_mode_box, QOverload<int>::of(&QComboBox::currentIndexChanged),
			this, [=](){ emit altered(get_state()); });
	connect(slider, &QSlider::valueChanged,
			spinbox, &QSpinBox::setValue);
	connect(spinbox, QOverload<int>::of(&QSpinBox::valueChanged),
			slider, &QSlider::setValue);
	
	// set the values after the connections to update other components
	paint_mode_box->addItem("Overlay", effect_state::over);
	paint_mode_box->addItem("Replace", effect_state::replace);
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
	options->addWidget(paint_mode_box);
	options->addStretch(1);
}

void single_color_widget::load_state(effect_state &new_state) {
	if (new_state.s_type == effect_state::single_color) {
		stored_color->set_color(new_state.s_c.stored_color);
		
		for (int index=0; index < paint_mode_box->count(); ++index) {
			if (paint_mode_box->currentData().value<effect_state::paint_mode>()
				== new_state.p_mode) {
				paint_mode_box->setCurrentIndex(index);
			}
		}
	}
}

effect_state single_color_widget::get_state() {
	single_color_state s_c = {stored_color->color()};
	return {
		effect_state::single_color,
		s_c,
		paint_mode_box->currentData().value<effect_state::paint_mode>()
	};
}
// end single_color


// gradient
gradient_widget::gradient_widget() : effect_widget("Gradient") {
	color_1 = new color_label(Qt::cyan);
	color_2 = new color_label(Qt::yellow);
	direction_box = new QComboBox;
	direction_box->addItem("Horizontal", true);
	direction_box->addItem("Vertical", false);
	auto swap_b = new QToolButton;
	swap_b->setText("Swap colors");
	
	connect(color_1, &color_label::color_changed,
			this, [=](){ emit altered(get_state()); });
	connect(color_2, &color_label::color_changed,
			this, [=](){ emit altered(get_state()); });
	connect(direction_box, QOverload<int>::of(&QComboBox::currentIndexChanged),
			this, [=](){ emit altered(get_state()); });
	connect(swap_b, &QToolButton::clicked, this, &gradient_widget::swap_colors);
	
	options->addWidget(direction_box);
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

void gradient_widget::swap_colors() {
	QColor temp = color_1->color();
	color_1->set_color(color_2->color());
	color_2->set_color(temp);
}

void gradient_widget::load_state(effect_state &new_state) {
	if (new_state.s_type == effect_state::gradient) {
		color_1->set_color(new_state.g.color_1);
		color_2->set_color(new_state.g.color_2);
		if (new_state.g.direction == gradient_state::horizontal)
			direction_box->setCurrentText("Horizontal");
		else
			direction_box->setCurrentText("Vertical");
	}
}

effect_state gradient_widget::get_state() {
	gradient_state g;
	g.color_1 = color_1->color();
	g.color_2 = color_2->color();
	bool hzon = direction_box->currentData().value<bool>();
	g.direction = (hzon) ? gradient_state::horizontal : gradient_state::vertical;
	return {effect_state::gradient, g};
}
// end gradient


// pixellate
pixellate_widget::pixellate_widget() : effect_widget("Pixellate") {
	pixel_size = new QSpinBox;
	pixel_size->setRange(1, 5000);
	pixel_size->setValue(5);
	
	connect(pixel_size, QOverload<int>::of(&QSpinBox::valueChanged),
			this, [=](){ emit altered(get_state()); });
	
	auto hbox = new QHBoxLayout;
	hbox->addWidget(new QLabel("Pixel Size:"));
	hbox->addStretch(1);
	hbox->addWidget(pixel_size);
	options->addLayout(hbox);
	options->addStretch(1);
}

void pixellate_widget::load_state(effect_state &new_state) {
	if (new_state.s_type == effect_state::pixellate)
		pixel_size->setValue(new_state.p.pixel_size);
}

effect_state pixellate_widget::get_state() {
	pixellate_state p = {pixel_size->value()};
	return {effect_state::pixellate, p};
}
// end pixellate
