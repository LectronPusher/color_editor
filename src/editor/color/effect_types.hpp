#pragma once

#include "effect.hpp"
#include "color_label.hpp"

#include <QComboBox>
#include <QGradient>

namespace editor {
namespace color {
namespace effect_types {

class solid_color : public effect {
public:
	solid_color();
	QImage create_mask(const QImage &image, const QRect &rect) override;
	
private:
	color_label *changeable_color;
	
}; // solid_color

class gradient : public effect {
public:
	gradient();
	QImage create_mask(const QImage &image, const QRect &rect) override;
	
private:
	QComboBox *orient_box;
	color_label *color_1;
	color_label *color_2;
	
	QGradient create_gradient(const QSize &size);
	
}; // gradient

} // effect_types
} // color
} // editor
