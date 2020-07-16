#include "selector_types.hpp"

#include <QToolButton>
#include <QLabel>

Q_DECLARE_METATYPE(QRegion::RegionType);

namespace editor {
namespace select {
namespace selector_types {

// select_all
select_all::select_all() : selector("Select All") {
	select_nothing = new QCheckBox("Select Nothing");
	auto exclude_b = new QToolButton;
	exclude_b->setText("Exclude");
	auto select_b = new QToolButton;
	select_b->setText("Select");
	
	connect(exclude_b, &QToolButton::clicked, this, [=](){
		emit selector::region_selected({ select(), selection::exclude });
	});
	connect(select_b, &QToolButton::clicked, this, [=](){
		emit selector::region_selected({ select(), selection::select });
	});
	
	options->addWidget(select_nothing, 0, 0, 1, 2);
	options->addWidget(exclude_b, 1, 0);
	options->addWidget(select_b, 1, 1);
	options->setRowStretch(2, 1);
}

QRegion select_all::select() {
	if (select_nothing->isChecked())
		return QRegion();
	return QRegion(image.rect());
}
// end select_all


// draw
draw::draw() : selector("Draw") {
	auto label = new QLabel("Size:");
	side_length = new QSpinBox;
	side_length->setRange(1, 1000);
	side_length->setValue(20);
	region_type = new QComboBox;
	region_type->addItem("Circle", QRegion::Ellipse);
	region_type->addItem("Square", QRegion::Rectangle);
	
	exclude_mm = new mouse_mode(mouse_mode::point, "Exclude");
	select_mm = new mouse_mode(mouse_mode::point, "Select");
	
	options->addWidget(label, 0, 0);
	options->addWidget(side_length, 0, 2);
	options->addWidget(region_type, 1, 0, 1, 3);
	options->addWidget(exclude_mm, 2, 0);
	options->addWidget(select_mm, 2, 1, 1, 2);
	options->setRowStretch(3, 1);
}

void draw::point_selected(const QPoint &point) {
	if (!exclude_mm->isChecked() && !select_mm->isChecked())
		return;
	
	QRect rect = create_rect(point);
	auto r_type = region_type->currentData().value<QRegion::RegionType>();
	QRegion new_region(rect, r_type);
	new_region = new_region.intersected(image.rect());
	
	auto s_type = (exclude_mm->isChecked()) ? selection::exclude : selection::select;
	emit selector::region_selected({ new_region, s_type});
}

QRect draw::create_rect(const QPoint &point) {
	int s = side_length->value();
	int b = (s % 2 == 0) ? (s - 1) /2 : s / 2;
	return QRect(point - QPoint(b, b), QSize(s, s));
}
// end draw

} // selector_types
} // select
} // editor
