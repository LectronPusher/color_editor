#include "select_panel.hpp"
#include "selector_types/types.hpp"

#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QToolButton>
#include <QDebug>

namespace editor {
namespace select {

select_panel::select_panel(QWidget *parent) : QWidget(parent) {
	// initializations
	stack = new selector_stack;
	auto combo_box = new QComboBox(this);
	// common buttons for all selectors
	auto select_b = new QToolButton(this);
	select_b->setText("Select");
	auto exclude_b = new QToolButton(this);
	exclude_b->setText("Exclude");
	
	// connections
	connect(select_b, &QToolButton::clicked, this, [this](){ which = select; });
	connect(select_b, &QToolButton::clicked, this, &select_panel::selection_requested);
	connect(exclude_b, &QToolButton::clicked, this, [this](){ which = exclude; });
	connect(exclude_b, &QToolButton::clicked, this, &select_panel::selection_requested);
	
	// selectors
	stack->add_selector(new selector_types::select_all(this));
	stack->setup_with_combo_box(combo_box);
	stack->setSizePolicy(QSizePolicy(QSizePolicy::Maximum, QSizePolicy::Maximum));
	
	// layout
	auto vbox = new QVBoxLayout;
	vbox->addWidget(combo_box);
	vbox->addWidget(stack);
	// common buttons
	auto hbox = new QHBoxLayout;
	hbox->addWidget(exclude_b);
	hbox->addWidget(select_b);
	vbox->addLayout(hbox);
	setLayout(vbox);
}

point_set select_panel::make_selection(const QImage &image) {
	point_set points = stack->active_selector()->select(image);
	point_set &set = (which == select) ? selection : exclusion;
	set |= points;
	return selected_points();
}

point_set select_panel::selected_points() {
	point_set points = selection;
	if (!exclusion.is_empty())
		points -= exclusion;
	return points;
}


} // select
} // editor
