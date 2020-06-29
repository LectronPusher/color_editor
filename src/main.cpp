#include <QApplication>
#include "editor/main_window.hpp"

int main(int argc, char **argv) {
	QApplication app(argc, argv);

	editor::main_window window;
	window.show();

	return app.exec();
}
