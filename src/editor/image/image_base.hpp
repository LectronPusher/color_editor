#pragma once

#include "mask.hpp"

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
	bool has_image() const;
	bool is_modified() const;
	const QImage apply_mask();
	void set_mask(const mask &new_mask);
	
private:
	QImage stored_image;
	mask image_mask;
	bool mask_applied = false;
	
}; // image_base

} // image
} // editor
