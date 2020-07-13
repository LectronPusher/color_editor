#pragma once

#include "image_base.hpp"

#include <QGraphicsView>
#include <QMouseEvent>
// #include <QPoint>
#include <QDir>
#include <QFileInfo>

namespace editor{
namespace image {

class image_view : public QGraphicsView {
	Q_OBJECT
	
public:
	enum mouse_mode {pan, color, point};
	
	image_view(QWidget *parent = nullptr);
	image_base *base();
	bool modifications_resolved();
	void set_mouse_mode(mouse_mode new_mode);
	
public slots:
	void open_image(QString filepath = QString());
	void save_as();
	
	void zoom_in();
	void zoom_out();
	void reset_zoom();
	
signals:
	void base_image_changed(QImage new_image);
	void point_selected(QPoint point);
	void color_selected(QColor color);
	
protected:
	void mouseMoveEvent(QMouseEvent *event);
	void mousePressEvent(QMouseEvent *event);
	
private:
	image_base *_base;
	mouse_mode mode;
	const qreal scale_factor = 1.2;
	
	QFileInfo old_file = QFileInfo("/home/ian/all/coding/c++/color_editor/data/");
// 	QFileInfo old_file = QFileInfo(QDir::homePath());
	
	void set_image(const QImage &image);
	
}; // image_view

} // image
} // editor
