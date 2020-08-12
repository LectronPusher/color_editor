#pragma once

#include "select_type.hpp"

#include <QRegion>

namespace editor {
namespace select {

struct region_pair { QRegion selected; QRegion excluded; };

struct change {
	change(select_type type, QRegion region);
	change(select_type type, region_pair regions);
	// boilerplate for union
	change(const change &other);
	change(change &&other);
	change operator=(const change &other);
	change operator=(change &&other);
	~change();
	
	select_type s_type;
	union {
		QRegion added_region;        // s_type == select/exclude
		region_pair removed_regions; // s_type == remove/clear
	};
private:
	void copy(const change &other);
	void move(const change &other);
}; // change

} // select
} // editor
