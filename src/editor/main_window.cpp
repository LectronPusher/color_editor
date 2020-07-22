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
	
	auto all_panels = new QHBoxLayout;
	all_panels->addLayout(image_panel);
	all_panels->addWidget(tool_panel_wrapper);
	all_panels->setAlignment(tool_panel_wrapper, Qt::AlignTop);
	setLayout(all_panels);
	// default so I don't have to manually load an image every time
	view->open_image("/home/ian/all/coding/c++/color_editor/data/mantis300.jpg");
}

void main_window::setup_image_panel(QVBoxLayout *panel_layout) {
	view = new image::image_view;
	mouse_mode::set_view(view);
	
	auto open_b = new QToolButton;
	open_b->setText("Open");
	auto save_as_b = new QToolButton;
	save_as_b->setText("Save As");
	auto zoom_in_b = new QToolButton;
	zoom_in_b->setText("Zoom In");
	auto zoom_out_b = new QToolButton;
	zoom_out_b->setText("Zoom Out");
	auto reset_zoom_b = new QToolButton;
	reset_zoom_b->setText("100%");
	
	connect(open_b, &QToolButton::clicked, view, [=](){
		view->open_image();
		selection.clear();
	});
	connect(save_as_b, &QToolButton::clicked, view, &image::image_view::save_as);
	connect(zoom_in_b, &QToolButton::clicked, view, &image::image_view::zoom_in);
	connect(zoom_out_b, &QToolButton::clicked, view, &image::image_view::zoom_out);
	connect(reset_zoom_b, &QToolButton::clicked, view, &image::image_view::reset_zoom);
	
	auto image_buttons = new QHBoxLayout;
	image_buttons->addWidget(open_b);
	image_buttons->addWidget(save_as_b);
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
	auto clear_b = new QToolButton;
	clear_b->setText("Clear Selection");
	
	connect(view, &image::image_view::base_image_changed,
			selector_stack->at(0), &select::selector::update_image);
	for (int i = 0; i < selector_stack->count(); ++i) {
		select::selector *sel = selector_stack->at(i);
		connect(sel, &select::selector::region_selected,
				this, &main_window::region_selected);
		connect(view, &image::image_view::point_selected,
				sel, &select::selector::point_selected);
	}
	connect(clear_b, &QToolButton::clicked, this, [=](){ selection.clear(); });
	connect(clear_b, &QToolButton::clicked, this, &main_window::effect_altered);
	
	panel_layout->addWidget(selector_stack);
	panel_layout->addWidget(remove_selection);
	panel_layout->setAlignment(remove_selection, Qt::AlignCenter);
	panel_layout->addWidget(clear_b);
	panel_layout->setAlignment(clear_b, Qt::AlignCenter);
}

void main_window::setup_color_panel(QVBoxLayout *panel_layout) {
	effect_stack = new widget_stack<color::effect>;
	effect_stack->add(new color::effect_types::solid_color);
	effect_stack->add(new color::effect_types::transparent);
	effect_stack->add(new color::effect_types::gradient);
	
	auto store_b = new QToolButton;
	store_b->setText("Save Effect to Image");
	
	for (int i = 0; i < effect_stack->count(); ++i) {
		connect(effect_stack->at(i), &color::effect::altered,
				this, &main_window::effect_altered);
	}
	connect(store_b, &QToolButton::clicked, view, [=](){
		const QImage &image = view->base()->apply_mask();
		selector_stack->at(0)->update_image(image);
		selection.clear();
	});
	
	panel_layout->addWidget(effect_stack);
	panel_layout->addWidget(store_b);
	panel_layout->setAlignment(store_b, Qt::AlignCenter);
}

void main_window::region_selected(select::selection::select_region region) {
	if (remove_selection->isChecked())
		region.second = select::selection::remove;
	selection.add(region);
	effect_altered();
}

void main_window::effect_altered() {
	const QImage &image = view->base()->image();
	const QRegion &region = selection.selected();
	image::mask mask = effect_stack->active()->create_mask(image, region);
	view->base()->set_mask(mask);
}

void main_window::closeEvent(QCloseEvent *event) {
	if (view->modifications_resolved())
		event->accept();
	else
		event->ignore();
}

} // editor
