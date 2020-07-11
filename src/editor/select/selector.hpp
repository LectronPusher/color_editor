#pragma once

#include <QWidget>
#include <QString>
#include <QImage>
#include <QGridLayout>
#include <QRegion>

namespace editor {
namespace select {

class selector : public QWidget {
	Q_OBJECT
	
public:
	selector(QWidget *parent, QString name_in);
	const QString name();
	virtual QRegion select(const QImage &image) const = 0;
	
protected:
	QGridLayout *options;
	
private:
	const QString selector_name;
	
}; // selector

} // select
} // editor
