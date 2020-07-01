#pragma once

#include "color_effect.hpp"

namespace editor {
namespace color {
namespace effect_types {

class make_red : public color_effect {
public:
	make_red(QWidget *parent);
	QImage create_mask(const QImage &image, const QRect &rect) const override;
}; // make_red

} // effect_types
} // color
} // editor
