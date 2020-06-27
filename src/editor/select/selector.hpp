#pragma once

#include "../point_set.hpp"

#include <QWidget>
#include <QString>
#include <QImage>
#include <QGridLayout>

namespace editor {
namespace select {

class selector : public QWidget {
	Q_OBJECT
public:
	selector(QWidget *parent, QString name_in);
	QString name;
	virtual point_set select(const QImage &image) const = 0;
protected:
	QGridLayout *options;
}; // selector

} // select
} // editor
