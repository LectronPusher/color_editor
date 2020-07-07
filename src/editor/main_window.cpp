#include "main_window.hpp"
#include "select/selector_types.hpp"
#include "color/effect_types.hpp"

#include <QGridLayout>
#include <QHBoxLayout>
#include <QLabel>
#include <QToolButton>
#include <QImage>
#include <QRegion>
#include <QDebug>

namespace editor {

main_window::main_window(QWidget *parent) : QWidget(parent) {
	setWindowTitle("color editor");
	
	QLabel *hline = new QLabel(this);
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
	tool_panels->setSizeConstraint(QLayout::SetFixedSize);
	
	auto all_panels = new QHBoxLayout;
	all_panels->addLayout(image_panel);
	all_panels->addWidget(tool_panel_wrapper);
	all_panels->setAlignment(tool_panel_wrapper, Qt::AlignTop);
	setLayout(all_panels);
}

void main_window::setup_image_panel(QVBoxLayout *image_panel) {
	view = new image::image_view(this);
	// default so I don't have to load an image during every test
// 	view->open_image("/home/ian/all/coding/c++/color_editor/data/mantis300.jpg");
	auto open_b = new QToolButton(this);
	open_b->setText("Open");
	auto save_as_b = new QToolButton(this);
	save_as_b->setText("Save As");
	auto zoom_in_b = new QToolButton(this);
	zoom_in_b->setText("Zoom In");
	auto zoom_out_b = new QToolButton(this);
	zoom_out_b->setText("Zoom Out");
	
	connect(open_b, &QToolButton::clicked, view, [=](){
		view->open_image(); 
		selection.clear();
	});
	connect(save_as_b, &QToolButton::clicked, view, &image::image_view::save_as);
	connect(zoom_in_b, &QToolButton::clicked, view, &image::image_view::zoom_in);
	connect(zoom_out_b, &QToolButton::clicked, view, &image::image_view::zoom_out);
	
	auto image_buttons = new QHBoxLayout;
	image_buttons->addWidget(open_b);
	image_buttons->addWidget(save_as_b);
	image_buttons->addWidget(zoom_in_b);
	image_buttons->addWidget(zoom_out_b);
	
	image_panel->addLayout(image_buttons);
	image_panel->setAlignment(image_buttons, Qt::AlignLeft);
	image_panel->addWidget(view);
}

// TODO clear the selection when an image is opened
void main_window::setup_select_panel(QVBoxLayout *select_panel) {
	selector_stack = new widget_stack<select::selector>(this);
	selector_stack->add(new select::selector_types::select_all(this));
	
	// common buttons for all selectors
	auto exclude_b = new QToolButton(this);
	exclude_b->setText("Exclude");
	auto select_b = new QToolButton(this);
	select_b->setText("Select");
	
	connect(exclude_b, &QToolButton::clicked,
			this, [this](){ selection.next_selection_type = select::exclude; });
	connect(exclude_b, &QToolButton::clicked, this, &main_window::select_points);
	connect(select_b, &QToolButton::clicked,
			this, [this](){ selection.next_selection_type = select::select; });
	connect(select_b, &QToolButton::clicked, this, &main_window::select_points);
	
	auto hbox = new QHBoxLayout;
	hbox->addWidget(exclude_b);
	hbox->addWidget(select_b);
	
	select_panel->addWidget(selector_stack);
	select_panel->addLayout(hbox);
}

void main_window::setup_color_panel(QVBoxLayout *color_panel) {
	effect_stack = new widget_stack<color::color_effect>(this);
	effect_stack->add(new color::effect_types::make_red(this));
	color_panel->addWidget(effect_stack);
}

void main_window::select_points() {
	const QImage image = view->get_image();
	if (!image.isNull()) {
		QRegion new_selection = selector_stack->active()->select(image);
		selection.add(new_selection);
		QRegion region = selection.selected();
		QImage mask =
			effect_stack->active()->create_mask(image, region.boundingRect());
		view->set_mask(mask, region);
	}
}

} // editor
