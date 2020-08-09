# pragma once

#include <QFrame>
#include <QColor>
#include <QColorDialog>
#include <QPointer>
#include <QPaintEvent>

namespace editor {
namespace color {

class color_label : public QFrame {
	Q_OBJECT
	
public:
	color_label(QColor starting_color = Qt::white, QWidget *parent = nullptr);
	virtual QSize sizeHint() const override;
	QColor color() const;
	void disable_interaction();
	void enable_transparency();
	
public slots:
	void set_color(const QColor &new_color);
	void set_alpha(int value);
	
signals:
	void color_changed(const QColor &new_color);
	void hidden(int r);
	
protected:
	virtual void paintEvent(QPaintEvent *event) override;
	virtual void hideEvent(QHideEvent *event) override;
	virtual void mousePressEvent(QMouseEvent *event) override;
	
private:
	QColor fill_color;
	bool can_be_transparent = false;
	bool is_interactive = true;
	
	QPointer<QColorDialog> dialog;
	
	void open_dialog();
	
}; // color_label

} // color
} // editor
