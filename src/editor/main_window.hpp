#pragma once

#include "editor_model.hpp"
#include "image/image_view.hpp"
#include "image/model_renderer.hpp"
#include "widget_stack.hpp"
#include "select/selector.hpp"
#include "color/effect.hpp"

#include <QWidget>
#include <QCloseEvent>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QToolButton>
#include <QButtonGroup>
#include <QFileInfo>
#include <QDir>

namespace editor {

class main_window : public QWidget {
	Q_OBJECT
	
public:
	main_window(QWidget *parent = nullptr);
	
public slots:
	void set_image(const QImage &image);
	void update_effect();
	void apply_mask();
	
	void open_image(QString filepath = QString());
	void save_as();
	
protected:
	void closeEvent(QCloseEvent *event);
	void keyPressEvent(QKeyEvent *event) override;
	
private:
	// all the data in one little bundle, shared with image_view and selectors
	editor_model *model;
	// a QGraphicsView that handles mouse input and zooming
	image::image_view *view;
	// communicates the mouse mode to view, used with selector checkboxes
	QButtonGroup *mouse_mode_group;
	// QGraphicsItem placed inside view, renders the image
	image::model_renderer *renderer;
	// selectors, these commnicate with view to create selection regions
	widget_stack<select::selector *> *selector_stack;
	// color effects, these are the effect that gets applied to the selected region
	widget_stack<color::effect *> *effect_stack;
	// applies current effect to the image, then resets to the new image
	QToolButton *apply_b;
	
	// which file was previously open, starts at the home directory
	QFileInfo previous_file = QDir::homePath();
	
	void initialize_members();
	void make_major_connections();
	void setup_layout();
	QHBoxLayout *create_image_buttons();
	
	bool modifications_resolved();
}; // main_window

} // editor
