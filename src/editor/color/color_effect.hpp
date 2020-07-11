#pragma once

#include <QWidget>
#include <QString>
#include <QImage>
#include <QRect>
#include <QGridLayout>

namespace editor {
namespace color {

class color_effect : public QWidget {
	Q_OBJECT
	
public:
	color_effect(QWidget *parent, QString name_in);
	const QString name() const;
	
public slots:
	virtual QImage create_mask(const QImage &image, const QRect &rect) const = 0;
	
signals:
	void altered();
	
protected:
	QGridLayout *options;
	
private:
	const QString effect_name;
	
}; // color_effect

} // color
} // editor
