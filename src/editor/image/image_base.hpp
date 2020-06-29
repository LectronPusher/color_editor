#pragma once

#include <QGraphicsItem>
#include <QStyleOptionGraphicsItem>
#include <QPainter>
#include <QImage>

namespace editor{
namespace image {

class image_base : public QGraphicsItem {
public:
	image_base(const QImage image_orig, QGraphicsItem *parent = nullptr);
	QRectF boundingRect() const override;
	// get the image
	const QImage get_image() const;
	void set_color_mask(const QImage color_mask, const QPoint initial_point);
	const QImage apply_mask();
	
	void paint(QPainter *painter, const QStyleOptionGraphicsItem *option,
			   QWidget *widget) override;
private:
	QImage image;
	QImage mask;
	QPoint mask_top_left;
	
}; // image_base

} // image
} // editor
