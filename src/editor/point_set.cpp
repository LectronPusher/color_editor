#include "point_set.hpp"

namespace editor {

void point_set::insert(const QPoint &point) {
	// I don't want to do size == 1, but ugh this sucks
	if (is_empty()) {
		all_points.insert(point);
		recalculate_rect();
	} else {
		all_points.insert(point);
	}
}

bool point_set::contains(const QPoint &point) const {
	return all_points.contains(point);
}

bool point_set::is_empty() const {
	return all_points.isEmpty();
}

QRect point_set::bounding_rect() const {
	if (is_empty())
		return QRect();
	else
		return QRect(QPoint(top, left), QPoint(bot, right));
}

void point_set::operator|=(const point_set &other) {
	all_points |= other.all_points;
	set_largest_rect(other);
}

void point_set::operator&=(const point_set &other) {
	all_points &= other.all_points;
	recalculate_rect();
}

void point_set::operator-=(const point_set &other) {
	all_points -= other.all_points;
	recalculate_rect();
}

void point_set::recalculate_rect() {
	if (!is_empty()) {
		auto iter = all_points.begin();
		left = iter->x();
		right = iter->x();
		bot = iter->y();
		top = iter->y();
		while (++iter != all_points.end())
			update_rect(* iter);
	}
}

void point_set::update_rect(const QPoint &point) {
	// left, right
	if (point.x() < left)
		left = point.x();
	else if (point.x() > right)
		right = point.x();
	// bot, top
	if (point.y() < bot)
		bot = point.y();
	else if (point.y() > top)
		top = point.y();
}

void point_set::set_largest_rect(const editor::point_set &other) {
	auto set_max = [](int &a, const int &b) { a = (a > b) ? a : b; };
	auto set_min = [](int &a, const int &b) { a = (a < b) ? a : b; };
	set_min(left, other.left);
	set_max(right, other.right);
	set_min(bot, other.bot);
	set_max(top, other.top);
}


}
