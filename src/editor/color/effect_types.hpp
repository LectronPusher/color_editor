#pragma once

#include "effect.hpp"
#include "color_label.hpp"
#include "../image/mask.hpp"
#include "../image/image_base.hpp"

#include <QComboBox>
#include <QGradient>
#include <QSpinBox>

namespace editor {
namespace color {
namespace effect_types {

class solid_color : public effect {
public:
	solid_color();
	image::mask create_mask(const QImage &image, const QRegion &region) override;
	
private:
	color_label *changeable_color;
	
}; // solid_color


class gradient : public effect {
	Q_OBJECT
	
public:
	gradient();
	image::mask create_mask(const QImage &image, const QRegion &region) override;
	
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
	image::mask create_mask(const QImage &image, const QRegion &region) override;
	
private slots:
	void set_label_transparency(int value);
	
private:
	color_label *trans_label;
	QComboBox *override_box;
	
}; // transparent


class pixellate : public effect {
public:
	pixellate();
	image::mask create_mask(const QImage &image, const QRegion &region) override;
	
private:
	QSpinBox *pixel_size;
	
	QRect create_rect(const QPoint &point);
	
}; // pixellate

} // effect_types
} // color
} // editor
