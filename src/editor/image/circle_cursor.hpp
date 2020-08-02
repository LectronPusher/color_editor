#pragma once

#include <QCursor>
#include <QBitmap>
#include <QPainter>

namespace editor {
namespace image {

// creates a mostly transparent cursor of 3 circles, inner 2 black, outer white
static QCursor circle_cursor() {
	QPoint center(16, 16);
	int r = 8;
	
	QBitmap mask = QBitmap(32, 32);
	mask.clear();
	QPainter painter(&mask);
	painter.setPen(Qt::color1);
	// idk why, but a QPen of width 3 doesn't work properly, oh well
	painter.drawEllipse(center, r + 1, r + 1);
	painter.drawEllipse(center, r, r);
	painter.drawEllipse(center, r - 1, r - 1);
	// the circles don't match perfectly, need to fill these 4 points:
	painter.drawPoint(center + QPoint(6, 6));
	painter.drawPoint(center + QPoint(6, -6));
	painter.drawPoint(center + QPoint(-6, 6));
	painter.drawPoint(center + QPoint(-6, -6));
	
	QBitmap pixels = QBitmap(32, 32);
	pixels.clear();
	QPainter painter2(&pixels);
	painter2.setPen(Qt::color1);
	painter2.drawEllipse(center, r, r);
	painter2.drawEllipse(center, r - 1, r - 1);
	
	return QCursor(pixels, mask);
}

} // image
} // editor
