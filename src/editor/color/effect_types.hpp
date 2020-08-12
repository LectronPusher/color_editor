#pragma once

#include "effect.hpp"
#include "color_label.hpp"

#include <QComboBox>
#include <QGradient>
#include <QSpinBox>
#include <QPainter>

namespace editor {
namespace color {
namespace effect_types {

class single_color : public effect {
	Q_OBJECT
	
public:
	single_color();
	QImage create_effect(const QImage &image, const QRect &) override;
	
private:
	color_label *stored_color;
	
}; // single_color


class gradient : public effect {
	Q_OBJECT
	
public:
	gradient();
	QImage create_effect(const QImage &image, const QRect &boundary) override;
	
private slots:
	void swap_colors();
	
private:
	QComboBox *orient_box;
	color_label *color_1;
	color_label *color_2;
	
	QGradient create_gradient(const QRect &rect);
	
}; // gradient


class pixellate : public effect {
	Q_OBJECT
	
public:
	pixellate();
	QImage create_effect(const QImage &image, const QRect &) override;
	
private:
	QSpinBox *pixel_size;
	
	void pixellate_image(QPainter *painter, const QImage &source);
	
	QRect create_rect(const QPoint &point);
	
}; // pixellate

} // effect_types
} // color
} // editor
