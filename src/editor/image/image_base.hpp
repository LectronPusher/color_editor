#pragma once

#include "../editor_model.hpp"

#include <QGraphicsItem>
#include <QPainter>
#include <QStyleOptionGraphicsItem>

namespace editor{
namespace image {

class image_base : public QGraphicsItem {
public:
	image_base(editor_model *model);
	
	QRectF boundingRect() const override;
	void paint(QPainter *painter, const QStyleOptionGraphicsItem *option,
			   QWidget *widget) override;
	
private:
	editor_model *model;
	
}; // image_base

} // image
} // editor
