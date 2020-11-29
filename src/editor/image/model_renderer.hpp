#pragma once

#include "../select/selection.hpp"
#include "../color/effect_state.hpp"
#include "../color/create_effect.hpp"

#include <QObject>
#include <QGraphicsItem>
#include <QPainter>
#include <QStyleOptionGraphicsItem>

namespace editor{
namespace image {

class model_renderer : public QGraphicsItem, public QObject {
public:
	model_renderer(select::selection *&sel, QImage &img);
	
	QRectF boundingRect() const override;
	void paint(QPainter *painter, const QStyleOptionGraphicsItem *,
			   QWidget *) override;
	
	void render_effect(QPainter *painter, bool use_background);
	bool no_effective_effect();
	
public slots:
	void update_effect_state(const color::effect_state &new_state);
	void create_effect_and_render();
	
private:
	select::selection *&selection;
	QImage &source_img;
	color::effect_state eff_state;
	QImage effect;
	
}; // model_renderer

} // image
} // editor
