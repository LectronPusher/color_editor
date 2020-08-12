#pragma once

#include "../painting_mode.hpp"

#include <QWidget>
#include <QVBoxLayout>

namespace editor {
namespace color {

class effect : public QWidget {
	Q_OBJECT
	
public:
	effect(QString name_in);
	const QString name() const;
	virtual QImage create_effect(const QImage &image, const QRect &boundary) = 0;
	
signals:
	void altered();
	void mode_changed(painting_mode::mode new_mode, QPrivateSignal);
	
protected slots:
	void set_mode(painting_mode::mode new_mode);
	
protected:
	QVBoxLayout *options;
	
	virtual void showEvent(QShowEvent *event) override;
	
private:
	painting_mode::mode mode = painting_mode::default_mode;
	const QString effect_name;
	
}; // color_effect

} // color
} // editor
