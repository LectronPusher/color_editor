#include "select_panel.hpp"

#include <QHBoxLayout>
#include <QComboBox>
#include <QToolButton>
#include <QDebug>

namespace editor {
namespace select {

select_panel::select_panel(QWidget *parent) : QWidget(parent) {
	// initializations
	selectors.push_back(new select_all(this));
	foreach(auto unique_selector, selectors)
		unique_selector->setVisible(false);
	// combo box
	auto combo_box = new QComboBox(this);
	combo_box->setSizePolicy(QSizePolicy(QSizePolicy::Maximum, QSizePolicy::Fixed));
	foreach(auto unique_selector, selectors)
		combo_box->addItem(unique_selector->name);
	// common buttons for all selectors
	auto select_b = new QToolButton(this);
	select_b->setText("Select");
	
	// connections
	connect(select_b, &QToolButton::clicked, this, &select_panel::selection_requested);
	// this accounts for currentIndexChanged being overloaded and compilers being dumb
	void (QComboBox::*index_changed)(int) = &QComboBox::currentIndexChanged;
	connect(combo_box, index_changed, this, &select_panel::set_active_selector);
	
	// layout
	vbox = new QVBoxLayout;
	// selectors
	vbox->addWidget(combo_box, 0, Qt::AlignTop);
	foreach(auto unique_selector, selectors)
		vbox->addWidget(unique_selector);
	// common buttons
	auto hbox = new QHBoxLayout;
	hbox->addWidget(select_b, 0, Qt::AlignRight | Qt::AlignTop);
	vbox->addLayout(hbox, Qt::AlignTop);
	setLayout(vbox);
}

point_set select_panel::make_selection(const QImage &image) {
	point_set points = selectors.at(active_selector)->make_selection(image);
	selection |= points;
	// TODO exclusion functionality
	
	return selected_points();
}

point_set select_panel::selected_points() {
	point_set points = selection;
	if (!exclusion.is_empty())
		points -= exclusion;
	return points;
}

void select_panel::set_active_selector(int index) {
	selectors.at(active_selector)->setVisible(false);
	active_selector = index;
	selectors.at(active_selector)->setVisible(true);
}

} // select
} // editor
