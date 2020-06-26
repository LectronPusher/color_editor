#pragma once

#include "../point_set.hpp"

#include <QWidget>
#include <QImage>
#include <QString>
#include <QVBoxLayout>
#include <QCheckBox>

namespace editor {
namespace select {

class selector : public QWidget {
	Q_OBJECT
public:
	selector(QWidget *parent, QString name);
	QString name;
	QVBoxLayout *options_box;
	virtual point_set make_selection(const QImage &image) const = 0;
}; // selector

class select_all : public selector {
public:
	select_all(QWidget *parent);
	point_set make_selection(const QImage &image) const override;
	QCheckBox *select_nothing;
}; // select_all


} // select
} // editor

