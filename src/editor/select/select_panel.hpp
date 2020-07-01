#pragma once

#include "../point_set.hpp"
#include "selector.hpp"
#include "../widget_stack.hpp"

#include <QWidget>
#include <QStackedLayout>

namespace editor {
namespace select {

enum select_type {select, exclude};

class select_panel : public QWidget {
	Q_OBJECT
	
public:
	select_panel(QWidget *parent);
	point_set make_selection(const QImage &image);
	
signals:
	void selection_requested();
	
private:
	widget_stack<selector> *stack;
	selector *active_selector();
	void add_to_combo_box(QComboBox *combo_box);
	
	select_type which;
	point_set selection;
	point_set exclusion;
	point_set selected_points();
	
}; // select_panel

} // select
} // editor
