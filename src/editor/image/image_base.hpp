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
	void paint(QPainter *painter, const QStyleOptionGraphicsItem *option,
			   QWidget *widget) override;
	
	const QImage image() const;
	bool has_image();
	bool is_modified();
	const QImage apply_mask();
	void set_mask(const QImage &new_mask, const QRegion &region);
	
private:
	QImage stored_image;
	QImage mask;
	QRegion mask_region;
	bool modified = false;
	
}; // image_base

} // image
} // editor
