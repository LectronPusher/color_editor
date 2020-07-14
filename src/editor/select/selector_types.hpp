#pragma once

#include "selector.hpp"
#include "../mouse_mode.hpp"

#include <QCheckBox>
#include <QButtonGroup>
#include <QSpinBox>

namespace editor {
namespace select {
namespace selector_types {

class select_all : public selector {
public:
	select_all();
	
private:
	QCheckBox *select_nothing;
	
	QRegion select();
	
}; // select_all

class draw : public selector {
public:
	draw();
	
public slots:
	virtual void point_selected(const QPoint &point) override;
	
private:
	mouse_mode *exclude_mm;
	mouse_mode *select_mm;
	QButtonGroup *group;
	QSpinBox *side_length;
	
	QRect create_rect(const QPoint &point);
	
}; // draw

} // selector_types
} // select
} // editor
