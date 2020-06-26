#pragma once

#include "../point_set.hpp"
#include "selector.hpp"

#include <QWidget>
#include <QVBoxLayout>
#include <QList>

namespace editor {
namespace select {

class select_panel : public QWidget {
	Q_OBJECT
	
public:
	select_panel(QWidget *parent);
	point_set make_selection(const QImage &image);
	
signals:
	void selection_requested();
	
private slots:
	void set_active_selector(int index);
	
private:
	QVBoxLayout *vbox;
	QList<selector *> selectors;
	int active_selector;
	point_set selection;
	point_set exclusion;
	point_set selected_points();
	
}; // select_panel

} // select
} // editor
