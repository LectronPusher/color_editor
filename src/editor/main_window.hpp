#pragma once

#include "widget_stack.hpp"
#include "image/image_view.hpp"
#include "select/selection.hpp"
#include "select/selector.hpp"
#include "color/effect.hpp"

#include <QWidget>
#include <QVBoxLayout>

namespace editor {

class main_window : public QWidget {
	Q_OBJECT
	
public:
	main_window(QWidget *parent = nullptr);
	
private slots:
	void select_points();
	void effect_altered();
	
private:
	// stores the image and handles rendering and mouse input for the image
	image::image_view *view;
	// stores the selected pixels
	select::selection selection;
	// stores the available selectors and their algorithms
	widget_stack<select::selector> *selector_stack;
	// stores the available color effects and their algorithms
	widget_stack<color::effect> *effect_stack;
	
	void setup_image_panel(QVBoxLayout *image_panel);
	void setup_select_panel(QVBoxLayout *select_panel);
	void setup_color_panel(QVBoxLayout *color_panel);
	
}; // main_window

} // editor
