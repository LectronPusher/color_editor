#pragma once

#include "editor_model.hpp"
#include "widget_stack.hpp"
#include "image/image_view.hpp"
#include "image/model_renderer.hpp"
#include "select/selector.hpp"
#include "color/effect.hpp"

#include <QWidget>
#include <QCloseEvent>
#include <QVBoxLayout>
#include <QButtonGroup>
#include <QCheckBox>
#include <QFileInfo>
#include <QDir>

namespace editor {

class main_window : public QWidget {
	Q_OBJECT
	
public:
	main_window(QWidget *parent = nullptr);
	
public slots:
	void region_selected(editor_model::select_region region);
	void reapply_effect();
	
	void update_image(const QImage &image);
	
	void open_image(QString filepath = QString());
	void save_as();
	
protected:
	void closeEvent(QCloseEvent *event);
	void keyPressEvent(QKeyEvent *event) override;
	
private:
	// all the data in one little bundle, shared with image_view and selectors
	editor_model *model;
	// handles mouse input and some image functions like zooming
	image::image_view *view;
	// communicates the current mouse mode to view,
	QButtonGroup *mode_button_group;
	// item stored inside view, handles rendering the image
	image::model_renderer *renderer;
	// stores the available selectors and their algorithms
	widget_stack<select::selector *> *selector_stack;
	// checkbox for selector option
	QCheckBox *remove_selection;
	// stores the available color effects and their algorithms
	widget_stack<color::effect *> *effect_stack;
	// which file was previously open, starts at the home directory
	QFileInfo previous_file = QDir::homePath();
	
	void setup_image_panel(QVBoxLayout *panel_layout);
	void setup_select_panel(QVBoxLayout *panel_layout);
	void setup_color_panel(QVBoxLayout *panel_layout);
	
	void make_major_connections();
	
	bool modifications_resolved();
}; // main_window

} // editor
