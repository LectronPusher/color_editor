#pragma once

#include "../select/selection.hpp"
#include "../painting_mode.hpp"

#include <QObject>
#include <QGraphicsItem>
#include <QPainter>
#include <QStyleOptionGraphicsItem>

namespace editor{
namespace image {

class model_renderer : public QGraphicsItem {
public:
	model_renderer(select::selection *&sel);
	
	QRectF boundingRect() const override;
	#define unused_args const QStyleOptionGraphicsItem *, QWidget *
	void paint(QPainter *painter, unused_args) override;
	
	void render_effect(QPainter *painter, bool use_background);
	bool no_effective_effect();
	
	void update_effect(const QImage &new_effect);
	void update_mode(painting_mode::mode new_mode);
	
	QImage source;
	select::selection *&selection;
	QImage effect;
	painting_mode::mode mode;
	
}; // model_renderer

} // image
} // editor
