#pragma once

#include "image_base.hpp"

#include <QWidget>
#include <QImage>
#include <QDir>
#include <QGraphicsScene>
#include <QGraphicsView>

namespace editor{
namespace image {

class image_view : public QGraphicsView {
	Q_OBJECT
	
public:
	image_view(QWidget *parent);
	
public slots:
	// file i/o
	void open_image();
	void set_image(QImage image);
	bool save_request();
	void save_as();
	// zoom
	void zoom_in();
	void zoom_out();
	void reset_zoom();
	// image data i/o
	
	// mouse stuff
	
	
protected:
// 	void mousePressEvent(QMouseEvent *event) override;
// 	void mouseMoveEvent(QMouseEvent *event) override;
// 	void mouseReleaseEvent(QMouseEvent *event) override;
	
private:
	//file i/o
	image_base *base = nullptr;
	QGraphicsScene *scene;
	bool has_image = false;
	bool image_modified = false;
	// zoom
	qreal current_scale = 1.0;
	const qreal scale_by = 1.2;
	// mouse stuff
// 	bool pan_enabled = false;
// 	bool tool_enabled = false;
	
}; // image_view

} // image
} // editor
