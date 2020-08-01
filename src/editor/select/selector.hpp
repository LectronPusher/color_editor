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
	static void update_image(const QImage &new_image);
	
public slots:
	virtual void point_selected(const QPoint &point);
	
signals:
	void region_selected(editor_model::select_region region);
	
protected:
	QVBoxLayout *options;
	static QImage image;
	static QSet<QRgb> color_table;
	
	static void update_color_table();
	
private:
	const QString selector_name;
	
}; // selector

} // select
} // editor
