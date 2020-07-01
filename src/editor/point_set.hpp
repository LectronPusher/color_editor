#pragma once

#include <QSet>
#include <QPoint>
#include <QRect>

// enable QSet to hold QPoints
inline uint qHash (const QPoint &key) {
	return qHash(static_cast<qint64>(key.x()) << 32 | key.y());
}

namespace editor {

// container for a set of QPoints that returns the bounding QRect
class point_set {
public:
	// insert a point
	void insert(const QPoint &point);
	// check if a point is in the set, does NOT check the rectangle
	bool contains(const QPoint &point) const;
	// c'mon, ask me. ask me what this function does. I dare you
	bool is_empty() const;
	// return the bounding rectangle of the points
	// returns a null QRect if empty
	QRect rect() const;
	// set operations: intersect, minus, union
	void operator&=(const point_set &other);
	void operator-=(const point_set &other);
	void operator|=(const point_set &other);
	
private:
	// huh, didn't expect to see a set of points in my point set, weird
	QSet<QPoint> all_points;
	// boundary ints
	int left, right, top, bot;
	// compare point to the boundary, update if the point is outside
	void update_rect(const QPoint &point);
	// if not empty, reset the boundary ints from scratch, O(n)
	void recalculate_rect();
	// make the boundary ints encompass both point sets
	// require neither this or other are empty
	void set_largest_rect(const point_set &other);
	
}; // point_set

} // editor

