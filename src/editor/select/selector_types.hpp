#pragma once

#include "../../point_set.hpp"
#include "../selector.hpp"

#include <QWidget>
#include <QImage>
#include <QCheckBox>

namespace editor {
namespace select {
namespace selector_types {

class select_all : public selector {
public:
	select_all(QWidget *parent);
	point_set select(const QImage &image) const override;
	QCheckBox *select_nothing;
}; // select_all

} // selector_types
} // select
} // editor

