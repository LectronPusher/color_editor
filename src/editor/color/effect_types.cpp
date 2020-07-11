#include "effect_types.hpp"

#include <QColor>
#include <QLabel>

namespace editor {
namespace color {
namespace effect_types {

// solid_color
solid_color::solid_color(QWidget *parent)
: color_effect(parent, "Solid Color") {
	changeable_color = new color_label(this, Qt::red, true);
	auto label = new QLabel(this);
	label->setText("Color:");
	
	connect(changeable_color, &color_label::color_changed, this, &color_effect::altered);
	
	options->addWidget(label, 0, 0);
	options->addWidget(changeable_color, 0, 1);
}

QImage solid_color::create_mask(const QImage &, const QRect &rect) const {
	QImage mask(rect.size(), QImage::Format_RGB32);
	mask.fill(changeable_color->color());
	return mask;
}
// end solid_color

} // effect_types
} // color
} // editor
