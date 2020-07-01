#include "main_window.hpp"
#include "select/selector_types.hpp"

#include <QGridLayout>
#include <QHBoxLayout>
#include <QLabel>
#include <QToolButton>
#include <QImage>
#include <QDebug>

namespace editor {

main_window::main_window(QWidget *parent) : QWidget(parent) {
	setWindowTitle("color editor");
	
	QLabel *hline = new QLabel(this);
	hline->setFrameStyle(QFrame::HLine);
	hline->setFixedHeight(5);
	hline->setSizePolicy(QSizePolicy(QSizePolicy::Preferred, QSizePolicy::Fixed));
	
	auto image_panel = new QVBoxLayout;
	setup_image_panel(image_panel);
	
	auto tool_panels = new QVBoxLayout;
	setup_select_panel(tool_panels);
	tool_panels->addWidget(hline);
// 	setup_color_panel(tool_panels);
	tool_panels->addStretch();
	
	auto all_panels = new QHBoxLayout;
	all_panels->addLayout(image_panel);
	all_panels->addLayout(tool_panels);
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
	
	connect(open_b, &QToolButton::clicked, view, [=](){ view->open_image(); });
	connect(save_as_b, &QToolButton::clicked, view, &image::image_view::save_as);
	connect(zoom_in_b, &QToolButton::clicked, view, &image::image_view::zoom_in);
	connect(zoom_out_b, &QToolButton::clicked, view, &image::image_view::zoom_out);
	
	auto image_buttons = new QHBoxLayout;
	image_buttons->addWidget(open_b);
	image_buttons->addWidget(save_as_b);
	image_buttons->addWidget(zoom_in_b);
	image_buttons->addWidget(zoom_out_b);
	image_buttons->addStretch();
	
	image_panel->addLayout(image_buttons);
	image_panel->addWidget(view);
}

void main_window::setup_select_panel(QVBoxLayout *select_panel) {
	selector_stack = new widget_stack<select::selector>(this);
	selector_stack->add(new select::selector_types::select_all(this));
	
	// common buttons for all selectors
	auto select_b = new QToolButton(this);
	select_b->setText("Select");
	auto exclude_b = new QToolButton(this);
	exclude_b->setText("Exclude");
	
	connect(select_b, &QToolButton::clicked,
			this, [this](){ selection.next_selection_type = select::select; });
	connect(select_b, &QToolButton::clicked, this, &main_window::select_points);
	connect(exclude_b, &QToolButton::clicked,
			this, [this](){ selection.next_selection_type = select::exclude; });
	connect(exclude_b, &QToolButton::clicked, this, &main_window::select_points);
	
	auto hbox = new QHBoxLayout;
	hbox->addWidget(exclude_b);
	hbox->addWidget(select_b);
	
	select_panel->addWidget(selector_stack);
	select_panel->addLayout(hbox);
}

void main_window::select_points() {
	const QImage image = view->get_image();
	if (!image.isNull()) {
		point_set points = selector_stack->active()->select(image);
		selection.add_selected(points);
	}
}

} // editor
