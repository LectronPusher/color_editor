# pragma once

#include <QWidget>
#include <QColor>
#include <QPaintEvent>
#include <QPainter>

namespace editor {
namespace color {

class color_label : public QWidget {
	Q_OBJECT
	
public:
	color_label(QWidget *parent, QColor starting_color = Qt::white);
	QColor color() const;
	void set_color(QColor new_color);
	virtual QSize sizeHint() const override;
	
	void set_interactive(bool interactive);
	
protected:
	virtual void paintEvent(QPaintEvent *event) override;
	virtual void mousePressEvent(QMouseEvent *event) override;
	
private slots:
	void open_dialog();
	
private:
	QColor fill_color;
	bool is_interactive = false;
	
}; // color_label


} // color
} // editor
