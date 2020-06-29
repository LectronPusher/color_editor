#include "main_window.hpp"

#include <QGridLayout>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QLabel>
#include <QToolButton>
#include <QImage>
#include <QDebug>

namespace editor {

main_window::main_window(QWidget *parent) : QWidget(parent) {
	setWindowTitle("color editor");
	
	// initialization
	view = new image::image_view(this);
	// default so I don't have to load an image during every test
// 	view->open_image("/home/ian/all/coding/c++/color_editor/data/mantis300.jpg");
	select_pan = new select::select_panel(this);
// 	color_pan = new color::color_panel(this, tools::color);
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
	// selection and rendering
	connect(select_pan, &select::select_panel::selection_requested,
			this, &main_window::select_points);
// 	connect(this, &main_window::points_selected,
// 			view, &image::image_view::render_selected);
	// image buttons
	connect(open_b, &QToolButton::clicked, view, [=](){ view->open_image(); });
	connect(save_as_b, &QToolButton::clicked, view, &image::image_view::save_as);
	connect(zoom_in_b, &QToolButton::clicked, view, &image::image_view::zoom_in);
	connect(zoom_out_b, &QToolButton::clicked, view, &image::image_view::zoom_out);
	
	// layout stuff
	// image controls
	auto image_buttons = new QHBoxLayout;
	image_buttons->addWidget(open_b);
	image_buttons->addWidget(save_as_b);
	image_buttons->addWidget(zoom_in_b);
	image_buttons->addWidget(zoom_out_b);
	image_buttons->addStretch();
	// image panel
	auto image_panel = new QVBoxLayout;
	image_panel->addLayout(image_buttons);
	image_panel->addWidget(view);
	// tool panels
	auto tool_panels = new QVBoxLayout;
	tool_panels->addWidget(select_pan);
	// separator
	QLabel *hline = new QLabel(this);
	hline->setFrameStyle(QFrame::HLine);
	hline->setFixedHeight(5);
	hline->setSizePolicy(QSizePolicy(QSizePolicy::Preferred, QSizePolicy::Fixed));
	tool_panels->addWidget(hline);
	tool_panels->addStretch();
// 	tools_panel->addWidget(color_pan);
	// combine 'em
	auto all_panels = new QHBoxLayout;
	all_panels->addLayout(image_panel);
	all_panels->addLayout(tool_panels);
	setLayout(all_panels);
}

void main_window::select_points() {
	const QImage image = view->get_image();
	if (!image.isNull()) {
		point_set points = select_pan->make_selection(image);;
		emit points_selected(points);
	}
}

} // editor
