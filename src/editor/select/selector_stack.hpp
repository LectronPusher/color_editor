#pragma once

#include "selector.hpp"

#include <QStackedWidget>
#include <QStringList>
#include <QComboBox>

namespace editor {
namespace select {

class selector_stack : public QStackedWidget {
	Q_OBJECT
	
public:
	void add_selector(selector *sel);
	selector *active_selector();
	void setup_with_combo_box(QComboBox *combo_box);
// 	virtual Qt::Orientations expandingDirections() const override;
}; // selector_stack

} // select
} // editor
