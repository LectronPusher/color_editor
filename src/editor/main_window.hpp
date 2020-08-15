#pragma once

#include "undo_base.hpp"
#include "select/selection.hpp"
#include "image/image_view.hpp"
#include "image/model_renderer.hpp"
#include "widget_stack.hpp"
#include "color/effect_types.hpp"
#include "select/selector_types.hpp"

#include <QWidget>
#include <QCloseEvent>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QToolButton>
#include <QButtonGroup>
#include <QFileInfo>
#include <QDir>

namespace editor {

struct editor_change {
	enum change_type {initial, stored} c_type;
	QImage orig;
	select::selection *sel;
}; // editor_change

class main_window : public QWidget, public undo_base<editor_change> {
	Q_OBJECT
	
public:
	main_window(QWidget *parent = nullptr);
	
public slots:
	void undo();
	void redo();
	
	void update_effect();
	void clear_undone();
	
	void open_image(QString filepath = QString());
	void save_as();
	
protected:
	void closeEvent(QCloseEvent *event) override;
	void keyPressEvent(QKeyEvent *event) override;
	
private:
	// the selection at undo_base.past_changes.top(), referenced by renderer
	select::selection *cur_selection = nullptr;
	// QGraphicsItem placed inside view, renders the image
	image::model_renderer *renderer;
	// a QGraphicsView that handles mouse input and zooming
	image::image_view *view;
	// communicates the mouse mode to view, used with selector checkboxes
	QButtonGroup *mouse_mode_group;
	// selectors, these commnicate with view to create selection regions
	widget_stack<select::selector *> *selector_stack;
	// color effects, these are the effect that gets applied to the selected region
	widget_stack<color::effect *> *effect_stack;
	// applies current effect to the image, then resets to the new image
	QToolButton *apply_b;
	
	// which file was previously open, starts at the home directory
	QFileInfo previous_file = QDir::homePath();
	// the initial image from QImage(previous_file).convert(ARGB32)
	QImage initial_image;
	
	void initialize_members();
	void make_major_connections();
	void connect_selection(select::selection *sel);
	void disconnect_selection(select::selection *sel);
	void setup_layout();
	QHBoxLayout *create_image_buttons();
	
	void initialize_image(const QImage &image);
	void store_current_effect();
	void apply(const editor_change &chng) override;
	void unapply(const editor_change &chng) override;
	void set_image(const QImage &image);
	
	// called in main_window::undo/redo based on selection undo/redo state
	using undo_base<editor_change>::undo;
	using undo_base<editor_change>::redo;
	
	bool modifications_resolved();
}; // main_window

} // editor
