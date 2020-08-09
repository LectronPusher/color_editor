#pragma once

#include "../select/selection.hpp"
#include "../painting_mode.hpp"

#include <QGraphicsItem>
#include <QPainter>
#include <QStyleOptionGraphicsItem>

namespace editor{
namespace image {

class model_renderer : public QGraphicsItem {
public:
	model_renderer(select::selection *selection_in);
	
	QRectF boundingRect() const override;
	#define unused_args const QStyleOptionGraphicsItem *, QWidget *
	void paint(QPainter *painter, unused_args) override;
	
	void render(QPainter *painter, bool use_background);
	
	QImage source;
	select::selection *selection;
	QImage mask;
	painting_mode::mode mode;
	
}; // model_renderer

} // image
} // editor
