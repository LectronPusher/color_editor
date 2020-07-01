#include "effect_types.hpp"

#include <QColor>

namespace editor {
namespace color {
namespace effect_types {

make_red::make_red(QWidget *parent) : color_effect(parent, "Make Red") {}

QImage make_red::create_mask(const QImage &image, const QRect &rect) const {
	QImage mask(rect.size(), QImage::Format_RGB32);
	mask.fill(QColorConstants::Red);
	return mask;
}

} // effect_types
} // color
} // editor
