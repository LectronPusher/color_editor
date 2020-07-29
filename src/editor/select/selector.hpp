#pragma once

#include "../editor_model.hpp"

#include <QWidget>
#include <QImage>
#include <QVBoxLayout>
#include <QRegion>
#include <QSet>

namespace editor {
namespace select {

class selector : public QWidget {
	Q_OBJECT
	
public:
	selector(QString name_in);
	const QString name() const;
	static void set_model(editor_model *new_model);
	
public slots:
	virtual void point_selected(const QPoint &point);
	
signals:
	void region_selected(editor_model::select_region region);
	
protected:
	QVBoxLayout *options;
	static editor_model *model;
	
private:
	const QString selector_name;
	
}; // selector

} // select
} // editor
