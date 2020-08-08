#pragma once

#include "../editor_model.hpp"

#include <QWidget>
#include <QButtonGroup>
#include <QVBoxLayout>
#include <QSet>

namespace editor {
namespace select {

class selector : public QWidget {
	Q_OBJECT
	
public:
	selector(QString name_in);
	const QString name() const;
	virtual void add_checkboxes_to_group(QButtonGroup *) {}
	
	static void set_image(const QImage &new_image);
	
public slots:
	virtual void point_selected(const QPoint &) {}
	
signals:
	void region_selected(QRegion region, editor_model::select_type s_type);
	
protected:
	QVBoxLayout *options;
	static QImage image;
	static QSet<QRgb> color_table;
	
private:
	const QString selector_name;
	
	static void update_color_table();
	
}; // selector

} // select
} // editor
