#include "selector_types.hpp"

#include "ColorUtils.hpp"
#include "../mouse_mode.hpp"

#include <QHBoxLayout>
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
select_all::select_all() : selector("Select All") {
	auto exclude_b = tool_button_text("Exclude");
	auto select_b = tool_button_text("Select");
	
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
	exclude_cb = new QCheckBox("Exclude");
	select_cb = new QCheckBox("Select");
	auto group = new QButtonGroup(this);
	group->addButton(exclude_cb);
	group->addButton(select_cb);
	
	connect(exclude_cb, &QCheckBox::stateChanged, this, &draw::set_last_checkbox);
	connect(select_cb, &QCheckBox::stateChanged, this, &draw::set_last_checkbox);
	
	auto hbox = new QHBoxLayout;
	hbox->addWidget(new QLabel("Size:"));
	hbox->addWidget(side_length);
	side_length->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
	options->addLayout(hbox);
	options->addWidget(region_type);
	hbox = new QHBoxLayout;
	hbox->addWidget(exclude_cb);
	hbox->addWidget(select_cb);
	options->addLayout(hbox);
	options->addStretch(1);
}

void draw::point_selected(const QPoint &point) {
	if (!exclude_cb->isChecked() && !select_cb->isChecked())
		return;
	
	QRect rect = create_rect(point);
	auto r_type = region_type->currentData().value<QRegion::RegionType>();
	QRegion new_region(rect, r_type);
	new_region = new_region.intersected(image.rect());
	
	if (exclude_cb->isChecked())
		emit selector::region_selected({editor_model::exclude, new_region});
	else
		emit selector::region_selected({editor_model::select, new_region});
}

QRect draw::create_rect(const QPoint &point) {
	int s = side_length->value();
	// make point center if odd, top left of central square if even
	int b = (s % 2 == 0) ? (s - 1) /2 : s / 2;
	return {point - QPoint(b, b), QSize(s, s)};
}

void draw::add_checkboxes_to_group(QButtonGroup *group) {
	group->addButton(exclude_cb, mouse_mode::combined_points);
	group->addButton(select_cb, mouse_mode::combined_points);
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
	auto exclude_b = tool_button_text("Exclude");
	auto select_b = tool_button_text("Select");
	
	connect(exclude_b, &QToolButton::clicked, this, [=](){
		emit selector::region_selected({editor_model::exclude, matching_pixels()});
	});
	connect(select_b, &QToolButton::clicked, this, [=](){
		emit selector::region_selected({editor_model::select, matching_pixels()});
	});
	
	auto hbox = new QHBoxLayout;
	hbox->addWidget(choose_color_cb);
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
