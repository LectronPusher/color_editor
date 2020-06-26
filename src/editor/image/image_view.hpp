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
	QSize minimumSizeHint() const override;
	
public slots:
	// file i/o
	void open_image(QString filepath = "");
	bool save_request();
	void save_as();
	// zoom
	void zoom_in();
	void zoom_out();
	// image data i/o
	QImage get_image();
	// mouse stuff
	
protected:
// 	void mousePressEvent(QMouseEvent *event) override;
// 	void mouseMoveEvent(QMouseEvent *event) override;
// 	void mouseReleaseEvent(QMouseEvent *event) override;
	
private:
	//file i/o
	image_base *base = nullptr;
	QGraphicsScene *image_scene;
	bool has_image = false;
	bool image_modified = false;
	// zoom
	const qreal scale_by = 1.2;
	// mouse stuff
// 	bool pan_enabled = false;
// 	bool tool_enabled = false;
	
	void set_image(QImage image);
	
}; // image_view

} // image
} // editor
