#pragma once

#include <QGraphicsItem>
#include <QStyleOptionGraphicsItem>
#include <QPainter>
#include <QImage>
#include <QBitmap>

namespace editor{
namespace image {

// contains the main image and a mask showing the selection and applied color
// the mask is a transparent layer over the image except where selected
// this also contains helper functions for reading and writing from the image
class image_base : public QGraphicsItem {
public:
	image_base(QImage image_orig);
	
	QRectF boundingRect() const override;
	
	void paint(QPainter *painter, const QStyleOptionGraphicsItem *option,
			   QWidget *widget) override;
	
	QImage get_image();
	
private:
	QImage image;
	QImage mask;
	
}; // image_base

} // image
} // editor
