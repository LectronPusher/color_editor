#include "selector_types.hpp"

#include "ColorUtils.hpp"
#include "../mouse_mode.hpp"

#include <QHBoxLayout>
#include <QGridLayout>
#include <QToolButton>
#include <QLabel>
#include <QBitmap>

Q_DECLARE_METATYPE(QRegion::RegionType);

using namespace editor::select::selector_types;

static QToolButton *tool_button_text(const QString &text) {
	auto button = new QToolButton;
	button->setText(text);
	return button;
}

// select_all
select_all::select_all() : selector("Entire Image") {
	auto select_b = tool_button_text("Select");
	auto clear_b = tool_button_text("Clear");
	
	connect(select_b, &QToolButton::clicked, this, [=](){
		emit selector::region_selected(image.rect(), editor_model::select);
	});
	connect(clear_b, &QToolButton::clicked, this, [=](){
		emit selector::region_selected(QRegion(), editor_model::clear);
	});
	
	options->addStretch(1);
	auto hbox = new QHBoxLayout;
	hbox->addWidget(select_b);
	hbox->addWidget(clear_b);
	options->addLayout(hbox);
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
	select_cb = new QCheckBox("Select");
	remove_cb = new QCheckBox("Remove");
	exclude_cb = new QCheckBox("Exclude");
	auto group = new QButtonGroup(this);
	group->addButton(select_cb);
	group->addButton(remove_cb);
	group->addButton(exclude_cb);
	auto clear_b = tool_button_text("Clear");
	
	connect(select_cb, &QCheckBox::stateChanged, this, &draw::set_last_checkbox);
	connect(remove_cb, &QCheckBox::stateChanged, this, &draw::set_last_checkbox);
	connect(exclude_cb, &QCheckBox::stateChanged, this, &draw::set_last_checkbox);
	connect(clear_b, &QToolButton::clicked, this, [=](){
		emit selector::region_selected(QRegion(), editor_model::clear);
	});
	
	auto hbox = new QHBoxLayout;
	hbox->addWidget(new QLabel("Size:"));
	hbox->addWidget(side_length);
	side_length->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
	options->addLayout(hbox);
	options->addWidget(region_type);
	options->addStretch(1);
	
	auto grid = new QGridLayout;
	grid->addWidget(select_cb, 0, 0);
	grid->addWidget(remove_cb, 0, 1);
	grid->addWidget(exclude_cb, 1, 0);
	grid->addWidget(clear_b, 1, 1);
	options->addLayout(grid);
}

void draw::point_selected(const QPoint &point) {
	QRect rect = create_rect(point);
	auto r_type = region_type->currentData().value<QRegion::RegionType>();
	QRegion new_region(rect, r_type);
	new_region = new_region.intersected(image.rect());
	
	editor_model::select_type s_type;
	if (select_cb->isChecked())       s_type = editor_model::select;
	else if (remove_cb->isChecked())  s_type = editor_model::remove;
	else if (exclude_cb->isChecked()) s_type = editor_model::exclude;
	else return;
	
	emit selector::region_selected(new_region, s_type);
}

QRect draw::create_rect(const QPoint &point) {
	int s = side_length->value();
	// make point center if odd, top left of central square if even
	int b = (s % 2 == 0) ? (s - 1) /2 : s / 2;
	return {point - QPoint(b, b), QSize(s, s)};
}

void draw::add_checkboxes_to_group(QButtonGroup *group) {
	group->addButton(select_cb, mouse_mode::combined_points);
	group->addButton(remove_cb, mouse_mode::combined_points);
	group->addButton(exclude_cb, mouse_mode::combined_points);
}

void draw::set_last_checkbox(int state) {
	if (state == Qt::Checked)
		last_checkbox = qobject_cast<QCheckBox *>(sender());
}

void draw::showEvent(QShowEvent *) {
	if (last_checkbox != nullptr)
		last_checkbox->setChecked(true);
}
// end draw


// color_match
color_match::color_match() : selector("Match Colors") {
	source_color = new color::color_label;
	choose_color_cb = new QCheckBox("Choose Color");
	fuzziness = new QSpinBox;
	fuzziness->setRange(0, 100);
	fuzziness->setValue(10);
	fuzziness->setSuffix("%");
	auto select_b = tool_button_text("Select");
	auto remove_b = tool_button_text("Remove");
	auto exclude_b = tool_button_text("Exclude");
	auto clear_b = tool_button_text("Clear");
	
	connect(select_b, &QToolButton::clicked, this, [=](){
		emit selector::region_selected(matching_pixels(), editor_model::select);
	});
	connect(remove_b, &QToolButton::clicked, this, [=](){
		emit selector::region_selected(matching_pixels(), editor_model::remove);
	});
	connect(exclude_b, &QToolButton::clicked, this, [=](){
		emit selector::region_selected(matching_pixels(), editor_model::exclude);
	});
	connect(clear_b, &QToolButton::clicked, this, [=](){
		emit selector::region_selected(QRegion(), editor_model::clear);
	});
	
	auto hbox = new QHBoxLayout;
	hbox->addWidget(choose_color_cb);
	hbox->addStretch(1);
	hbox->addWidget(source_color);
	options->addLayout(hbox);
	hbox = new QHBoxLayout;
	hbox->addWidget(new QLabel("Fuzzy:"));
	hbox->addStretch(1);
	hbox->addWidget(fuzziness);
	options->addLayout(hbox);
	options->addStretch(1);
	
	auto grid = new QGridLayout;
	grid->addWidget(select_b, 0, 0);
	grid->addWidget(remove_b, 0, 1);
	grid->addWidget(exclude_b, 1, 0);
	grid->addWidget(clear_b, 1, 1);
	options->addLayout(grid);
}

void color_match::point_selected(const QPoint &point) {
	if (choose_color_cb->isChecked()) {
		if (image.rect().contains(point)) {
			QColor color;
			color.setRgba(image.pixel(point));
			source_color->set_color(color);
		}
	}
}

QRegion color_match::matching_pixels() {
	int fuzzy = fuzziness->value();
	QRgb source = source_color->color().rgba();
	
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

void color_match::add_checkboxes_to_group(QButtonGroup *group) {
	group->addButton(choose_color_cb, mouse_mode::single_point);
}

void color_match::showEvent(QShowEvent *) {
	choose_color_cb->setChecked(true);
}
// end color_match
