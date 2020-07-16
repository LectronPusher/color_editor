#pragma once

#include "../image/mask.hpp"

#include <QWidget>
#include <QImage>
#include <QRegion>
#include <QGridLayout>

namespace editor {
namespace color {

class effect : public QWidget {
	Q_OBJECT
	
public:
	effect(QString name_in);
	const QString name() const;
	virtual image::mask create_mask(const QImage &image, const QRegion &region) = 0;
	
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
