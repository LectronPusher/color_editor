#include "effect_types.hpp"

#include <QColor>

namespace editor {
namespace color {
namespace effect_types {

make_red::make_red(QWidget *parent) : color_effect(parent, "Make Red") {
	options->addWidget(new color_label(this, Qt::red), 0, 0);
}

QImage make_red::create_mask(const QImage &, const QRect &rect) const {
	QImage mask(rect.size(), QImage::Format_RGB32);
	mask.fill(fill_color->color());
	return mask;
}

} // effect_types
} // color
} // editor
