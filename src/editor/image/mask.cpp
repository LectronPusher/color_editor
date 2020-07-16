#include "mask.hpp"

namespace editor {
namespace image {

mask::mask() : mask(QImage(), QRegion(), false) {}

mask::mask(const QImage &image_in, const QRegion &region_in, bool paint_over_in)
: image(image_in), region(region_in), paint_over(paint_over_in) {}

QRect mask::rect() const {
	return region.boundingRect();
}

bool mask::is_valid() const {
	return !image.isNull();
}

void mask::paint_on(QPainter *painter, const QBrush &background) const {
	if (is_valid()) {
		painter->save();
		painter->setClipRegion(region);
		if (paint_over) {
			painter->setCompositionMode(QPainter::CompositionMode_Source);
			if (background != Qt::NoBrush) {
				painter->fillRect(rect(), background);
				painter->setCompositionMode(QPainter::CompositionMode_SourceOver);
			}
		}
		painter->drawImage(rect(), image);
		painter->restore();
	}
}

} // image
} // editor
