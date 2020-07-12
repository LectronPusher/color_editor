#pragma once

#include "image_base.hpp"

#include <QGraphicsView>
#include <QGraphicsScene>
#include <QDir>
#include <QFileInfo>

namespace editor{
namespace image {

class image_view : public QGraphicsView {
	Q_OBJECT
	
public:
	image_view(QWidget *parent = nullptr);
	image_base *base();
	bool modifications_resolved();
	
public slots:
	void open_image(QString filepath = QString());
	void save_as();
	
	void zoom_in();
	void zoom_out();
	void reset_zoom();
	
private:
	image_base *_base;
	const qreal scale_factor = 1.2;
	
	QFileInfo old_file = QFileInfo("/home/ian/all/coding/c++/color_editor/data/");
// 	QFileInfo old_file = QFileInfo(QDir::homePath());
	
	void set_image(const QImage &image);
	
}; // image_view

} // image
} // editor
