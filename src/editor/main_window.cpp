#include "main_window.hpp"

#include <QGridLayout>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QToolButton>
#include <QDebug>

namespace editor {

main_window::main_window(QWidget *parent) : QWidget(parent) {
	setWindowTitle("color editor");
	
	// initialization
	view = new image::image_view(this);
	QImage image("/home/ian/all/coding/c++/color_editor/data/mantis300.jpg");
	view->set_image(image);
	color_pan = new tools::tool_panel(this, tools::color);
	select_pan = new tools::tool_panel(this, tools::select);
	// buttons to control the image
	auto open_b = new QToolButton(this);
	open_b->setText("Open");
	auto save_as_b = new QToolButton(this);
	save_as_b->setText("Save As");
	auto zoom_in_b = new QToolButton(this);
	zoom_in_b->setText("Zoom In");
	auto zoom_out_b = new QToolButton(this);
	zoom_out_b->setText("Zoom Out");
	
	// connections
	connect(open_b, &QToolButton::clicked, view, &image::image_view::open_image);
	connect(save_as_b, &QToolButton::clicked, view, &image::image_view::save_as);
	connect(zoom_in_b, &QToolButton::clicked, view, &image::image_view::zoom_in);
	connect(zoom_out_b, &QToolButton::clicked, view, &image::image_view::zoom_out);
	
	// layout stuff
	// buttons
	auto image_buttons = new QHBoxLayout;
	image_buttons->addWidget(open_b);
	image_buttons->addWidget(save_as_b);
	image_buttons->addWidget(zoom_in_b);
	image_buttons->addWidget(zoom_out_b);
	image_buttons->addStretch();
	// images
	auto image_panel = new QVBoxLayout;
	image_panel->addLayout(image_buttons);
	image_panel->addWidget(view);
	// tool panels
	auto tools_panel = new QVBoxLayout;
	tools_panel->addWidget(select_pan);
	tools_panel->addWidget(color_pan);
	// comine 'em
	auto both = new QHBoxLayout;
	both->addLayout(image_panel);
	both->addLayout(tools_panel);
	setLayout(both);
}


} // editor
