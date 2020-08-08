#pragma once

#include "selector.hpp"
#include "../color/color_label.hpp"

#include <QCheckBox>
#include <QSpinBox>
#include <QComboBox>
#include <QBitmap>

namespace editor {
namespace select {
namespace selector_types {

class select_all : public selector {
	Q_OBJECT
	
public:
	select_all();
	
}; // select_all


class draw : public selector {
	Q_OBJECT
	
public:
	draw();
	void add_checkboxes_to_group(QButtonGroup *group) override;
	
public slots:
	void point_selected(const QPoint &point) override;
	void set_last_checkbox(int state);
	
protected:
	virtual void showEvent(QShowEvent *event) override;
	
private:
	QCheckBox *select_cb;
	QCheckBox *remove_cb;
	QCheckBox *exclude_cb;
	QCheckBox *last_checkbox = nullptr;
	QSpinBox *side_length;
	QComboBox *region_type;
	
	QRect create_rect(const QPoint &point);
	
}; // draw


class color_match : public selector {
	Q_OBJECT
	
public:
	color_match();
	
public slots:
	void point_selected(const QPoint &point) override;
	void add_checkboxes_to_group(QButtonGroup *group) override;
	
protected:
	virtual void showEvent(QShowEvent *event) override;
	
private:
	color::color_label *source_color;
	QCheckBox *choose_color_cb;
	QCheckBox *last_checkbox = nullptr;
	QSpinBox *fuzziness;
	
	QRegion matching_pixels();
	QBitmap matching_bitmap(const QRgb &rgb);
	
}; // color_match

} // selector_types
} // select
} // editor
