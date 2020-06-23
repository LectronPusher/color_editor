#include "./mainwindow.hpp"
#include "image/imagepanel.hpp"
#include "tools/toolpanel.hpp"

#include <QGridLayout>

#include <QTextStream>
QTextStream out1(stdout);

editor::main_window::main_window() {
	setWindowTitle("color editor");
	
	// initialization
	image_pan = new editor::image::image_panel(this);
	//image_pan->load_image("../data/mantis300.jpg");
	color_pan = new editor::tools::tool_panel(this, editor::tools::color);
	select_pan = new editor::tools::tool_panel(this, editor::tools::select);
	
	// connections
	
	
	// layout stuff
	QGridLayout *grid = new QGridLayout;
	grid->addWidget(image_pan, 0, 0, 2, 1);
	grid->addWidget(color_pan, 1, 0);
	grid->addWidget(select_pan, 1, 1);
	setLayout(grid);
}
