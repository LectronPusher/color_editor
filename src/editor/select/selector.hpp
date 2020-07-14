#pragma once

#include "selection.hpp"

#include <QWidget>
#include <QImage>
#include <QGridLayout>
#include <QRegion>

namespace editor {
namespace select {

class selector : public QWidget {
	Q_OBJECT
	
public:
	selector(QString name_in);
	const QString name() const;
	
public slots:
	void update_image(const QImage &new_image);
	virtual void point_selected(const QPoint &point);
	
signals:
	void region_selected(selection::select_region region);
	
protected:
	QGridLayout *options;
	static QImage image;
	
private:
	const QString selector_name;
	
}; // selector

} // select
} // editor
