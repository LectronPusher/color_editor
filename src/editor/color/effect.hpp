#pragma once

#include <QWidget>
#include <QString>
#include <QImage>
#include <QRect>
#include <QGridLayout>

namespace editor {
namespace color {

class effect : public QWidget {
	Q_OBJECT
	
public:
	effect(QString name_in);
	const QString name() const;
	virtual QImage create_mask(const QImage &image, const QRect &rect) = 0;
	
signals:
	void altered();
	
protected:
	QGridLayout *options;
	virtual void showEvent(QShowEvent *event) override;
	
private:
	const QString effect_name;
	
}; // color_effect

} // color
} // editor
