#pragma once

#include <QGraphicsItem>
#include <QImage>
#include <QPainter>
#include <QStyleOptionGraphicsItem>
#include <QRegion>

namespace editor{
namespace image {

class image_base : public QGraphicsItem {
public:
	image_base(const QImage image_orig, QGraphicsItem *parent = nullptr);
	QRectF boundingRect() const override;
	
	const QImage get_image() const;
	void set_mask(const QImage &new_mask, const QRegion &region);
	const QImage apply_mask();
	
	void paint(QPainter *painter, const QStyleOptionGraphicsItem *option,
			   QWidget *widget) override;
	
private:
	QImage image;
	QImage mask;
	QRegion mask_region;
	
}; // image_base

} // image
} // editor
