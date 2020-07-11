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
	QSize minimumSizeHint() const override;
	const QImage get_image() const;
	void set_mask(const QImage &new_mask, const QRegion &region);
	
public slots:
	void open_image(QString filepath = QString());
	void save_as();
	bool maybe_save();
	
	void zoom_in();
	void zoom_out();
	void reset_zoom();
	
private:
	image_base *base;
	bool has_image = false;
	bool image_modified = false;
	const qreal scale_factor = 1.2;
	
	QFileInfo old_file = QFileInfo("/home/ian/all/coding/c++/color_editor/data/");
// 	QFileInfo old_file = QFileInfo(QDir::homePath());
	
	void set_image(const QImage &image);
	
}; // image_view

} // image
} // editor
