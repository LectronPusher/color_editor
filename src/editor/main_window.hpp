#pragma once

#include "editor_model.hpp"
#include "widget_stack.hpp"
#include "image/image_view.hpp"
#include "select/selector.hpp"
#include "color/effect.hpp"

#include <QWidget>
#include <QCloseEvent>
#include <QVBoxLayout>
#include <QCheckBox>

namespace editor {

class main_window : public QWidget {
	Q_OBJECT
	
public:
	main_window(QWidget *parent = nullptr);
	
public slots:
	void region_selected(editor_model::select_region region);
	void effect_altered();
	
protected:
	void closeEvent(QCloseEvent *event);
	
private:
	// all the data in one little bundle, shared with image_view
	editor_model *model;
	// stores the image and handles rendering and mouse input for the image
	image::image_view *view;
	// stores the available selectors and their algorithms
	widget_stack<select::selector> *selector_stack;
	// checkbox for selector option
	QCheckBox *remove_selection;
	// stores the available color effects and their algorithms
	widget_stack<color::effect> *effect_stack;
	
	void setup_image_panel(QVBoxLayout *panel_layout);
	void setup_select_panel(QVBoxLayout *panel_layout);
	void setup_color_panel(QVBoxLayout *panel_layout);
	
}; // main_window

} // editor
