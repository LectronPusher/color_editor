#pragma once

#include "color_effect.hpp"
#include "color_label.hpp"

namespace editor {
namespace color {
namespace effect_types {

class solid_color : public color_effect {
public:
	solid_color(QWidget *parent);
	QImage create_mask(const QImage &image, const QRect &rect) const override;
	
private:
	color_label *changable_color;
	
}; // solid_color

} // effect_types
} // color
} // editor
