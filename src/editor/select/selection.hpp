#pragma once

#include <QObject>
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
	struct change {
		change(select_type type, QRegion region);
		change(select_type type, region_pair regions);
		// boilerplate for union
		change(const change &other);
		change operator=(const change &other);
		~change();
		
		select_type s_type;
		union {
			QRegion added_region;
			region_pair removed_regions;
		};
	}; // change
	
public:
	selection(QObject *parent);
	
	QRegion selected_region() const;
	QRect region_rect() const;
	bool has_selection() const;
	
public slots:
	void add_region(const QRegion &region, select_type s_type);
	void undo();
	void redo();
	void combine_changes(int n);
	
signals:
	void contents_updated(QRect new_contents_rect);
	void boundary_updated(QRect new_boundary);
	
private:
	QRegion selected;
	QRegion excluded;
	QRegion combined;
	QRect combined_boundary;
	
	std::forward_list<change> applied;
	std::forward_list<change> undone;
	
	void apply_change(const change &data);
	void undo_change(const change &data);
	void update_combined();
	
}; // selection

} // select
} // editor
