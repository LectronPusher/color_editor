#pragma once

#include "../editor_model.hpp"

#include <QWidget>
#include <QVBoxLayout>

namespace editor {
namespace color {

class effect : public QWidget {
	Q_OBJECT
	
public:
	effect(QString name_in);
	const QString name() const;
	virtual editor_model::mask_pair create_mask(const QImage &image,
												const QRect &rect) = 0;
	
signals:
	void altered();
	
protected:
	QVBoxLayout *options;
	virtual void showEvent(QShowEvent *event) override;
	
private:
	const QString effect_name;
	
}; // color_effect

} // color
} // editor
