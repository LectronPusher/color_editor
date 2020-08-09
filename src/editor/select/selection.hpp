#pragma once

#include <QObject>
#include <QImage>
#include <QRegion>

#include <forward_list>

namespace editor {
namespace select {

class selection : public QObject {
	Q_OBJECT
	
public:
	enum select_type {select, exclude, remove, clear};
	
private:
	struct region_pair { QRegion selected; QRegion excluded; };
	struct region_change {
		region_change(select_type type, QRegion region);
		region_change(select_type type, region_pair regions);
		// boilerplate for union
		region_change(const region_change &other);
		region_change operator=(const region_change &other);
		~region_change();
		
		select_type s_type;
		union {
			QRegion added_region;
			region_pair removed_regions;
		};
	}; // region_change
	
public:
	QRegion selected_region() const;
	QRect region_rect() const;
	bool has_selection() const;
	
public slots:
	void add_region(const QRegion &region, select_type s_type);
	void clear_regions();
	void undo();
	void redo();
	void combine_recent_changes(int n);
	
signals:
	void contents_updated(QRect new_contents_rect);
	void region_boundary_updated(QRect new_boundary);
	
private:
	QRegion selected;
	QRegion excluded;
	QRegion combined;
	
	std::forward_list<region_change> applied_changes;
	std::forward_list<region_change> undone_changes;
	
	void apply_change(const region_change &data);
	void update_combined();
	
}; // selection

} // select
} // editor
