#pragma once

#include "selector.hpp"

#include <QWidget>
#include <QImage>
#include <QCheckBox>
#include <QRegion>

namespace editor {
namespace select {
namespace selector_types {

class select_all : public selector {
public:
	select_all();
	QRegion select(const QImage &image) override;
	
private:
	QCheckBox *select_nothing;
	
}; // select_all

} // selector_types
} // select
} // editor
