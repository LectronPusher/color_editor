#include "change.hpp"

using namespace editor::select;

change::change(select_type type, QRegion region)
: s_type(type), added_region(region) {}

change::change(select_type type, region_pair regions)
: s_type(type), removed_regions(regions) {}

change::change(const change &other) {
	copy(other);
}

change::change(change &&other) {
	move(other);
}

change change::operator=(const change &other) {
	copy(other);
	return *this;
}

change change::operator=(change &&other) {
	move(other);
	return *this;
}

void change::copy(const change &other) {
	s_type = other.s_type;
	switch (s_type) {
		case remove:
		case clear:
			new(&removed_regions) region_pair(other.removed_regions);
			break;
		case select:
		case exclude:
			new(&added_region) QRegion(other.added_region);
			break;
	}
}

void change::move(const change &other) {
	s_type = std::move(other.s_type);
	switch (s_type) {
		case remove:
		case clear:
			new(&removed_regions) region_pair(std::move(other.removed_regions));
			break;
		case select:
		case exclude:
			new(&added_region) QRegion(std::move(other.added_region));
			break;
	}
}

change::~change() {
	switch (s_type) {
		case remove:
		case clear:
			removed_regions.~region_pair();
			break;
		case select:
		case exclude:
			added_region.~QRegion();
			break;
	}
}
