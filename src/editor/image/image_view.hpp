#pragma once

#include "../editor_model.hpp"
#include "image_base.hpp"

#include <QGraphicsView>
#include <QMouseEvent>
#include <QDir>
#include <QFileInfo>

namespace editor{
namespace image {

class image_view : public QGraphicsView {
	Q_OBJECT
	
public:
	image_view(editor_model *model_in, QWidget *parent = nullptr);
	bool modifications_resolved();
	
public slots:
	void open_image(QString filepath = QString());
	void save_as();
	
	void update_rect(const QRect &scene_rect);
	
	void zoom_in();
	void zoom_out();
	void reset_zoom();
	
	void update_mouse_mode();
	
signals:
	void point_selected(QPoint point);
	
protected:
	void mouseMoveEvent(QMouseEvent *event);
	void mousePressEvent(QMouseEvent *event);
	
private:
	editor_model *model;
	const qreal scale_factor = 1.2;
	
	QFileInfo old_file = QFileInfo("/home/ian/all/coding/c++/color_editor/data/");
// 	QFileInfo old_file = QFileInfo(QDir::homePath());
	
	QPoint scene_point(const QPoint &pos);
	void set_image(const QImage &image);
	
}; // image_view

} // image
} // editor
