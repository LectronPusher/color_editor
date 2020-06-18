#include <QApplication>
#include <QMainWindow>

int main(int argc, char **argv) {
	QApplication app(argc, argv);

	QMainWindow window;
	window.setWindowTitle("color editor");
	window.show();

	return app.exec();
}
