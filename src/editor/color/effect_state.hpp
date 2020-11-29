#pragma once

#include <QColor>

namespace editor {
namespace color {

struct single_color_state {
	QColor stored_color;
};

struct gradient_state {
	QColor color_1 = Qt::cyan;
	QColor color_2 = Qt::yellow;
	enum {horizontal, vertical} direction = horizontal;
};

struct pixellate_state {
	int pixel_size;
};

struct effect_state {
	enum state_type {single_color, gradient, pixellate} s_type;
	enum paint_mode {replace, over} p_mode;
	union {
		single_color_state s_c;
		gradient_state g;
		pixellate_state p;
	};
	
	// boilerplate for union
	effect_state();
	effect_state(state_type type, single_color_state s_c, paint_mode p_m = replace);
	effect_state(state_type type, gradient_state g, paint_mode p_m = replace);
	effect_state(state_type type, pixellate_state p, paint_mode p_m = replace);
	effect_state(const effect_state &other);
	effect_state(effect_state &&other);
	effect_state operator=(const effect_state &other);
	effect_state operator=(effect_state &&other);
	~effect_state();
	
private:
	void copy(const effect_state &other);
	void move(const effect_state &other);
}; // effect_state

} // color
} // editor
