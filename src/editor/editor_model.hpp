#pragma once

#include "painting_mode.hpp"

#include <QObject>
#include <QImage>
#include <QRegion>
#include <QPainter>

#include <forward_list>

namespace editor {

class editor_model : public QObject {
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
	const QImage source_image();
	const QRect image_rect();
	const QRegion selected_region();
	const QRect region_rect();
	bool is_altered();
	
public slots:
	void set_image(const QImage &new_image);
	
	void set_mask(const QImage& new_mask, painting_mode::mode new_mode);
	void apply_mask();
	void draw_mask(QPainter *painter, bool use_background);
	
	void add_region(const QRegion &region, select_type s_type);
	void clear_regions();
	void undo();
	void redo();
	void combine_recent_changes(int n);
	
signals:
	void image_changed(const QImage &new_image);
	void contents_updated(QRect new_contents_rect);
	void region_boundary_updated(QRect new_boundary);
	
private:
	QImage image;
	
	QImage mask;
	painting_mode::mode mode;
	
	QRegion selected;
	QRegion excluded;
	QRegion combined;
	
	std::forward_list<region_change> applied_changes;
	std::forward_list<region_change> undone_changes;
	
	void apply_change(const region_change &data);
	void update_combined();
	
}; // editor_model

} // editor

Q_DECLARE_METATYPE(editor::editor_model::select_type);
