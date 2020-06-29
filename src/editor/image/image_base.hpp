#pragma once

#include <QGraphicsItem>
#include <QStyleOptionGraphicsItem>
#include <QPainter>
#include <QImage>
#include <QBitmap>

namespace editor{
namespace image {

class image_base : public QGraphicsItem {
public:
	image_base(QImage image_orig);
	
	QRectF boundingRect() const override;
	
	void paint(QPainter *painter, const QStyleOptionGraphicsItem *option,
			   QWidget *widget) override;
	
	QImage get_image();
	
private:
	QImage image;
	
}; // image_base

} // image
} // editor
