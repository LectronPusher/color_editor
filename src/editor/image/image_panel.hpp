#pragma once

#include "./image_base.hpp"

#include <QWidget>
#include <QImage>
#include <QDir>
#include <QGraphicsScene>
#include <QGraphicsView>

namespace editor{
namespace image {

class image_panel : public QWidget {
	Q_OBJECT
	
public:
	image_panel(QWidget *parent);
	
public slots:
	void open_image(bool oof = false);
	void set_image(QImage image);
	bool save_request();
	void save_as();
	void zoom_in();
	void zoom_out();
	
private:
	image_base *base = nullptr;
	QGraphicsScene *scene;
	QGraphicsView *view;
	
	bool has_image = false;
	bool image_modified = false;
	QString image_path;
	
}; // image_panel

} // image
} // editor
