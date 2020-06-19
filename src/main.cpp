#include <QApplication>
#include "editor/mainwindow.hpp"

int main(int argc, char **argv) {
	QApplication app(argc, argv);

	editor::main_window window;
	window.setWindowTitle("color editor");
	window.show();

	return app.exec();
}

