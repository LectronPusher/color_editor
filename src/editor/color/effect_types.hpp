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

class solid_color : public effect {
	Q_OBJECT
	
public:
	solid_color();
	QImage create_mask(editor_model *model) override;
	
private:
	color_label *changeable_color;
	
}; // solid_color


class gradient : public effect {
	Q_OBJECT
	
public:
	gradient();
	QImage create_mask(editor_model *model) override;
	
private slots:
	void swap_colors();
	
private:
	QComboBox *orient_box;
	color_label *color_1;
	color_label *color_2;
	
	QGradient create_gradient(const QSize &size);
	
}; // gradient


class transparent : public effect {
	Q_OBJECT
	
public:
	transparent();
	editor_model::painting_mode paint_mode() override;
	QImage create_mask(editor_model *model) override;
	
private slots:
	void set_label_transparency(int value);
	
private:
	color_label *trans_label;
	QComboBox *override_box;
	
}; // transparent


class pixellate : public effect {
	Q_OBJECT
	
public:
	pixellate();
	QImage create_mask(editor_model *model) override;
	
private:
	QSpinBox *pixel_size;
	
	editor_model::painting_mode paint_mode() override;
	void pixellate_image(QPainter *painter, const QImage &source);
	
	QRect create_rect(const QPoint &point);
	
}; // pixellate

} // effect_types
} // color
} // editor
