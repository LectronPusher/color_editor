#pragma once

#include <QPainter>
#include <QRegion>
#include <QImage>

namespace editor {
namespace image {

class mask {
public:
	mask();
	mask(const QImage &image_in, const QRegion &region_in, bool paint_over_in = false);
	QRect rect() const;
	bool is_valid() const;
	void paint_on(QPainter *painter, const QBrush &background = Qt::NoBrush) const;
	
private:
	QImage image;
	QRegion region;
	bool paint_over = false;
	
}; // mask

} // image
} // editor
