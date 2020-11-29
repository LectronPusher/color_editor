#pragma once

#include "effect_widget.hpp"
#include "color_label.hpp"

#include <QComboBox>
#include <QSpinBox>

// used by single_color_widget paint_mode_box
Q_DECLARE_METATYPE(editor::color::effect_state::paint_mode);

namespace editor {
namespace color {

class single_color_widget : public effect_widget {
	Q_OBJECT
	
public:
	single_color_widget();
	void load_state(effect_state &new_state) override;
	effect_state get_state() override;
	
private:
	color_label *stored_color;
	QComboBox *paint_mode_box;
	
}; // single_color_widget


class gradient_widget : public effect_widget {
	Q_OBJECT
	
public:
	gradient_widget();
	void load_state(effect_state &new_state) override;
	effect_state get_state() override;
	
private slots:
	void swap_colors();
	
private:
	QComboBox *direction_box;
	color_label *color_1;
	color_label *color_2;
	
}; // gradient_widget


class pixellate_widget : public effect_widget {
	Q_OBJECT
	
public:
	pixellate_widget();
	void load_state(effect_state &new_state) override;
	effect_state get_state() override;
	
private:
	QSpinBox *pixel_size;
	
}; // pixellate_widget

} // color
} // editor
