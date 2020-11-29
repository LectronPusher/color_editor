#include "create_effect.hpp"

#include <QLinearGradient>

namespace editor {
namespace color {

QImage create_effect(const effect_state &state, const QImage &image,
					 const QRect &boundary) {
	QImage effect(image.size(), QImage::Format_ARGB32);
	
	switch (state.s_type) {
		case effect_state::single_color:
			paint_single_color(effect, state.s_c);
			break;
		case effect_state::gradient:
			paint_gradient(effect, state.g, boundary);
			break;
		case effect_state::pixellate:
			paint_pixellate(effect, state.p, image);
			break;
	}
	return effect;
}

void paint_single_color(QImage &effect, single_color_state s_c) {
	effect.fill(s_c.stored_color);
}

void paint_gradient(QImage &effect, gradient_state g, const QRect &boundary) {
	QPainter painter(&effect);
	QLinearGradient grad;
	grad.setColorAt(0, g.color_1);
	grad.setColorAt(1, g.color_2);
	grad.setStart(boundary.left(), boundary.top());
	
	if (g.direction == gradient_state::horizontal)
		grad.setFinalStop(boundary.right(), boundary.top());
	else
		grad.setFinalStop(boundary.left(), boundary.bottom());
	
	painter.fillRect(effect.rect(), grad);
}

void paint_pixellate(QImage &effect, pixellate_state p, const QImage &source) {
	if (p.pixel_size == 1)
		return; // nothing to do
	QPainter painter(&effect);
	painter.setCompositionMode(QPainter::CompositionMode_Source);
	
	int size = p.pixel_size;
	int h_s = (size - 1) / 2; // half of size, it works don't touch it
	// create a rect centered on the point with side lengths of size
	auto create_rect = [size, h_s] (const QPoint &point) {
		return QRect(point - QPoint(h_s, h_s), QSize(size, size));
	};
	
	int width = source.width();
	int height = source.height();
	
	// fill top left until run out of granularity
	for (int row = h_s; row < width; row += size) {
		for (int col = h_s; col < height; col += size) {
			QPoint point(row, col);
			painter.fillRect(create_rect(point), source.pixelColor(point));
		}
	}
	// prepare to fill the bottom and right
	int width_mod = (width % size <= h_s) ? width % size : 0;
	int height_mod = (height % size <= h_s) ? height % size : 0;
	QRect right(width - width_mod, 0, width_mod, height);
	QRect bottom(0, height - height_mod, width, height_mod);
	// fill right edge
	for (int col = h_s; col < height; col += size) {
		QPoint point(width - 1, col);
		painter.fillRect(right & create_rect(point), source.pixelColor(point));
	}
	// fill bottom edge
	for (int row = h_s; row < width; row += size) {
		QPoint point(row, height - 1);
		painter.fillRect(bottom & create_rect(point), source.pixelColor(point));
	}
	// fill bottom right corner
	painter.fillRect(right & bottom, source.pixelColor(width - 1, height - 1));
}

}; // color
}; // editor
