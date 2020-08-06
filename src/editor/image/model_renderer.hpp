#pragma once

#include "../editor_model.hpp"

#include <QGraphicsItem>
#include <QPainter>
#include <QStyleOptionGraphicsItem>

namespace editor{
namespace image {

class model_renderer : public QGraphicsItem {
public:
	model_renderer(editor_model *model);
	
	QRectF boundingRect() const override;
	void paint(QPainter *painter, const QStyleOptionGraphicsItem *option,
			   QWidget *widget) override;
	editor_model *model;
	
}; // model_renderer

} // image
} // editor
