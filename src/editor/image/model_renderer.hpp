#pragma once

#include "../editor_model.hpp"
#include "../painting_mode.hpp"

#include <QGraphicsItem>
#include <QPainter>
#include <QStyleOptionGraphicsItem>

namespace editor{
namespace image {

class model_renderer : public QGraphicsItem {
public:
	model_renderer(editor_model *model);
	
	QRectF boundingRect() const override;
	#define unused_args const QStyleOptionGraphicsItem *, QWidget *
	void paint(QPainter *painter, unused_args) override;
	
	void render(QPainter *painter, bool use_background);
	
	editor_model *model;
	QImage mask;
	painting_mode::mode mode;
	
}; // model_renderer

} // image
} // editor
