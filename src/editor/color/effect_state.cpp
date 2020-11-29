#include "effect_state.hpp"

using namespace editor::color;

effect_state::effect_state(): s_type(effect_state::single_color),
p_mode(over), s_c({Qt::white}) {}

effect_state::effect_state(state_type type, single_color_state s_c, paint_mode p_m)
: s_type(type), p_mode(p_m), s_c(s_c) {}
effect_state::effect_state(state_type type, gradient_state g, paint_mode p_m)
: s_type(type), p_mode(p_m), g(g) {}
effect_state::effect_state(state_type type, pixellate_state p, paint_mode p_m)
: s_type(type), p_mode(p_m), p(p) {}

effect_state::effect_state(const effect_state &other) {
	copy(other);
}

effect_state::effect_state(effect_state &&other) {
	move(other);
}

effect_state effect_state::operator=(const effect_state &other) {
	copy(other);
	return *this;
}

effect_state effect_state::operator=(effect_state &&other) {
	move(other);
	return *this;
}

void effect_state::copy(const effect_state &other) {
	s_type = other.s_type;
	p_mode = other.p_mode;
	switch (s_type) {
		case effect_state::single_color:
			new(&s_c) single_color_state(other.s_c);
			break;
		case effect_state::gradient:
			new(&g) gradient_state(other.g);
			break;
		case effect_state::pixellate:
			new(&p) pixellate_state(other.p);
			break;
	}
}

void effect_state::move(const effect_state &other) {
	s_type = std::move(other.s_type);
	p_mode = std::move(other.p_mode);
	switch (s_type) {
		case effect_state::single_color:
			new(&s_c) single_color_state(std::move(other.s_c));
			break;
		case effect_state::gradient:
			new(&g) gradient_state(std::move(other.g));
			break;
		case effect_state::pixellate:
			new(&p) pixellate_state(std::move(other.p));
			break;
	}
}

effect_state::~effect_state() {
	switch (s_type) {
		case effect_state::single_color:
			s_c.~single_color_state();
			break;
		case effect_state::gradient:
			g.~gradient_state();
			break;
		case effect_state::pixellate:
			p.~pixellate_state();
			break;
	}
}
