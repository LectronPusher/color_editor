#pragma once

#include "change.hpp"
#include "../undo_base.hpp"

#include <QObject>

namespace editor {

class main_window;

namespace select {

class selection : public QObject, public undo_base<change> {
	Q_OBJECT
	friend main_window; // to clear undo_base::undone when altered
	
public:
	selection(QObject *parent);
	
	QRegion selected_region() const;
	QRect region_rect() const;
	bool has_selection() const;
	
private:
	// disallow the direct use of undo_base::add(), instead use add_region
	using undo_base<change>::add;
public slots:
	void add_region(select_type s_type, const QRegion &region);
	void combine_changes(int n);
	
signals:
	void contents_updated(QRect new_contents_rect);
	void boundary_updated(QRect new_boundary);
	void region_added();
	
private:
	QRegion selected;
	QRegion excluded;
	QRegion combined;
	QRect combined_boundary;
	
	void apply(const change &data) override;
	void unapply(const change &data) override;
	void update_combined();
	
}; // selection

} // select
} // editor
