#include "selector_types.hpp"

#include "ColorUtils.hpp"

#include <QHBoxLayout>
#include <QToolButton>
#include <QLabel>
#include <QBitmap>

Q_DECLARE_METATYPE(QRegion::RegionType);

namespace editor {
namespace select {
namespace selector_types {

// select_all
select_all::select_all() : selector("Select All") {
	auto exclude_b = new QToolButton;
	exclude_b->setText("Exclude");
	auto select_b = new QToolButton;
	select_b->setText("Select");
	
	connect(exclude_b, &QToolButton::clicked, this, [=](){
		emit selector::region_selected({editor_model::exclude, image.rect()});
	});
	connect(select_b, &QToolButton::clicked, this, [=](){
		emit selector::region_selected({editor_model::select, image.rect()});
	});
	
	auto hbox = new QHBoxLayout;
	hbox->addWidget(exclude_b);
	hbox->addWidget(select_b);
	options->addLayout(hbox);
	options->addStretch(1);
}
// end select_all


// draw
draw::draw() : selector("Draw") {
	side_length = new QSpinBox;
	side_length->setRange(1, 1000);
	side_length->setValue(20);
	region_type = new QComboBox;
	region_type->addItem("Circle", QRegion::Ellipse);
	region_type->addItem("Square", QRegion::Rectangle);
	
	exclude_mm = new mouse_mode(mouse_mode::combined_points, "Exclude");
	select_mm = new mouse_mode(mouse_mode::combined_points, "Select");
	
	auto hbox = new QHBoxLayout;
	hbox->addWidget(new QLabel("Size:"));
	hbox->addWidget(side_length);
	side_length->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
	options->addLayout(hbox);
	options->addWidget(region_type);
	hbox = new QHBoxLayout;
	hbox->addWidget(exclude_mm);
	hbox->addWidget(select_mm);
	options->addLayout(hbox);
	options->addStretch(1);
}

void draw::point_selected(const QPoint &point) {
	if (!exclude_mm->isChecked() && !select_mm->isChecked())
		return;
	
	QRect rect = create_rect(point);
	auto r_type = region_type->currentData().value<QRegion::RegionType>();
	QRegion new_region(rect, r_type);
	new_region = new_region.intersected(image.rect());
	
	if (exclude_mm->isChecked())
		emit selector::region_selected({editor_model::exclude, new_region});
	else
		emit selector::region_selected({editor_model::select, new_region});
}

QRect draw::create_rect(const QPoint &point) {
	int s = side_length->value();
	int b = (s % 2 == 0) ? (s - 1) /2 : s / 2;
	return {point - QPoint(b, b), QSize(s, s)};
}
// end draw


// color_match
color_match::color_match() : selector("Match Colors") {
	auto exclude_b = new QToolButton;
	exclude_b->setText("Exclude");
	auto select_b = new QToolButton;
	select_b->setText("Select");
	source_color = new color::color_label;
	choose_color = new mouse_mode(mouse_mode::single_point, "Choose Color");
	fuzziness = new QSpinBox;
	fuzziness->setRange(0, 100);
	fuzziness->setValue(10);
	fuzziness->setSuffix("%");
	
	connect(exclude_b, &QToolButton::clicked, this, [=](){
		emit selector::region_selected({editor_model::exclude, matching_pixels()});
	});
	connect(select_b, &QToolButton::clicked, this, [=](){
		emit selector::region_selected({editor_model::select, matching_pixels()});
	});
	
	auto hbox = new QHBoxLayout;
	hbox->addWidget(choose_color);
	hbox->addWidget(source_color);
	options->addLayout(hbox);
	hbox = new QHBoxLayout;
	hbox->addWidget(new QLabel("Fuzzy:"));
	hbox->addWidget(fuzziness);
	options->addLayout(hbox);
	hbox = new QHBoxLayout;
	hbox->addWidget(exclude_b);
	hbox->addWidget(select_b);
	options->addLayout(hbox);
	options->addStretch(1);
}

void color_match::point_selected(const QPoint &point) {
	if (choose_color->isChecked())
		if (image.rect().contains(point))
			source_color->set_color(image.pixelColor(point));
}

QRegion color_match::matching_pixels() {
	int fuzzy = fuzziness->value();
	QRgb source = source_color->color().rgb();
	
	QRegion region;
	if (fuzzy == 100)
		region = image.rect();
	else if (fuzzy == 0)
		region = matching_bitmap(source);
	else
		foreach (QRgb rgb, color_table)
			if (ColorUtils::getColorDeltaE(rgb, source) <= fuzzy)
				region |= matching_bitmap(rgb);
	return region;
}

QBitmap color_match::matching_bitmap(const QRgb &rgb) {
	QImage mask = image.createMaskFromColor(rgb, Qt::MaskOutColor);
	return QBitmap::fromImage(mask);
}
// end color_match

} // selector_types
} // select
} // editor
