#include "main_window.hpp"
#include "select/selector_types.hpp"
#include "color/effect_types.hpp"
#include "mouse_mode.hpp"

#include <QGridLayout>
#include <QHBoxLayout>
#include <QLabel>
#include <QToolButton>
#include <QDebug>

namespace editor {

main_window::main_window(QWidget *parent) : QWidget(parent) {
	setWindowTitle("color editor");
	
	model = new editor_model;
	select::selector::set_model(model);
	
	auto hline = new QLabel;
	hline->setFrameStyle(QFrame::HLine);
	hline->setFixedHeight(5);
	hline->setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Fixed);
	
	auto image_panel = new QVBoxLayout;
	setup_image_panel(image_panel);
	
	QWidget *tool_panel_wrapper = new QWidget;
	auto tool_panels = new QVBoxLayout(tool_panel_wrapper);
	tool_panels->setContentsMargins(0, 0, 0, 0);
	setup_select_panel(tool_panels);
	tool_panels->addWidget(hline);
	setup_color_panel(tool_panels);
	// requires tool_panel_wrapper, doesn't work without it
	tool_panels->setSizeConstraint(QLayout::SetFixedSize);
	
	// all major connections
	connect(model, &editor_model::boundary_rect_updated,
			this, &main_window::effect_altered);
	connect(model, &editor_model::contents_updated,
			view, &image::image_view::update_rect);
	// selectors
	for (int i = 0; i < selector_stack->count(); ++i) {
		select::selector *sel = selector_stack->at(i);
		connect(sel, &select::selector::region_selected,
				this, &main_window::region_selected);
		connect(view, &image::image_view::point_selected,
				sel, &select::selector::point_selected);
	}
	// effects
	for (int i = 0; i < effect_stack->count(); ++i) {
		connect(effect_stack->at(i), &color::effect::altered,
				this, &main_window::effect_altered);
	}
	
	auto all_panels = new QHBoxLayout;
	all_panels->addLayout(image_panel);
	all_panels->addWidget(tool_panel_wrapper);
	all_panels->setAlignment(tool_panel_wrapper, Qt::AlignTop);
	setLayout(all_panels);
	// default so I don't have to manually load an image every time
	view->open_image("/home/ian/all/coding/c++/color_editor/data/mantis300.jpg");
}

QToolButton *tool_button_text(const QString&text) {
	auto button= new QToolButton;
	button->setText(text);
	return button;
}

void main_window::setup_image_panel(QVBoxLayout *panel_layout) {
	view = new image::image_view(model);
	mouse_mode::set_view(view);
	
	auto open_b = tool_button_text("Open");
	auto save_as_b = tool_button_text("Save As");
	auto undo_b = tool_button_text("Undo");
	auto redo_b = tool_button_text("Redo");
	auto zoom_in_b = tool_button_text("Zoom In");
	auto zoom_out_b = tool_button_text("Zoom Out");
	auto reset_zoom_b = tool_button_text("100%");
	
	connect(open_b, &QToolButton::clicked, view, [=](){ view->open_image(); });
	connect(save_as_b, &QToolButton::clicked, view, &image::image_view::save_as);
	connect(undo_b, &QToolButton::clicked, model, &editor_model::undo);
	connect(redo_b, &QToolButton::clicked, model, &editor_model::redo);
	connect(zoom_in_b, &QToolButton::clicked, view, &image::image_view::zoom_in);
	connect(zoom_out_b, &QToolButton::clicked, view, &image::image_view::zoom_out);
	connect(reset_zoom_b, &QToolButton::clicked, view, &image::image_view::reset_zoom);
	
	auto image_buttons = new QHBoxLayout;
	image_buttons->addWidget(open_b);
	image_buttons->addWidget(save_as_b);
	image_buttons->addWidget(undo_b);
	image_buttons->addWidget(redo_b);
	image_buttons->addWidget(zoom_in_b);
	image_buttons->addWidget(zoom_out_b);
	image_buttons->addWidget(reset_zoom_b);
	image_buttons->addWidget(new mouse_mode(mouse_mode::pan, "Pan"));

	panel_layout->addLayout(image_buttons);
	panel_layout->setAlignment(image_buttons, Qt::AlignLeft);
	panel_layout->addWidget(view);
}

void main_window::setup_select_panel(QVBoxLayout *panel_layout) {
	selector_stack = new widget_stack<select::selector>;
	selector_stack->add(new select::selector_types::select_all);
	selector_stack->add(new select::selector_types::draw);
	selector_stack->add(new select::selector_types::color_match);
	
	remove_selection = new QCheckBox("Remove From Both");
	
	auto clear_b = tool_button_text("Clear Selection");
	connect(clear_b, &QToolButton::clicked, this, [=](){ model->clear_regions(); });
	
	panel_layout->addWidget(selector_stack);
	panel_layout->addWidget(remove_selection);
	panel_layout->setAlignment(remove_selection, Qt::AlignCenter);
	panel_layout->addWidget(clear_b);
	panel_layout->setAlignment(clear_b, Qt::AlignCenter);
}

void main_window::setup_color_panel(QVBoxLayout *panel_layout) {
	effect_stack = new widget_stack<color::effect>;
	effect_stack->add(new color::effect_types::transparent);
// 	effect_stack->add(new color::effect_types::solid_color);
	effect_stack->add(new color::effect_types::gradient);
	effect_stack->add(new color::effect_types::pixellate);
	
	auto store_b = tool_button_text("Save Effect to Image");
	connect(store_b, &QToolButton::clicked,
			this, [=](){ model->set_image(model->apply_mask()); });
	
	panel_layout->addWidget(effect_stack);
	panel_layout->addWidget(store_b);
	panel_layout->setAlignment(store_b, Qt::AlignCenter);
}

void main_window::region_selected(editor_model::select_region region) {
	if (remove_selection->isChecked())
		region.first = editor_model::remove;
	model->add_region(region);
}

void main_window::effect_altered() {
	const QImage &image = model->image();
	const QRect &rect = model->region_rect();
	auto mask = effect_stack->active()->create_mask(image, rect);
	model->set_mask(mask);
}

void main_window::closeEvent(QCloseEvent *event) {
	if (view->modifications_resolved())
		event->accept();
	else
		event->ignore();
}

void main_window::keyPressEvent(QKeyEvent *event) {
	switch (event->key()) {
		case Qt::Key_Z:
			if (event->modifiers() == Qt::ControlModifier)
				model->undo();
			else if (event->modifiers() == (Qt::ControlModifier | Qt::ShiftModifier))
				model->redo();
			break;
		case Qt::Key_Y:
			if (event->modifiers() == Qt::ControlModifier)
				model->redo();
			break;
		case Qt::Key_Undo:
			model->undo();
			break;
		case Qt::Key_Redo:
			model->redo();
			break;
		default:
			event->ignore();
			break;
	}
}

} // editor
