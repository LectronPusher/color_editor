# pragma once

#include <QFrame>
#include <QColor>
#include <QPaintEvent>
#include <QPainter>

namespace editor {
namespace color {

class color_label : public QFrame {
	Q_OBJECT
	
public:
	color_label(QWidget *parent, QColor starting_color = Qt::white,
				bool interactive  = false);
	virtual QSize sizeHint() const override;
	QColor color() const;
	
	bool is_interactive;
	
public slots:
	void set_color(const QColor &new_color);
	
signals:
	void color_changed(const QColor &new_color);
	
protected:
	virtual void paintEvent(QPaintEvent *event) override;
	virtual void mousePressEvent(QMouseEvent *event) override;
	
private:
	QColor fill_color;
	
	void open_dialog();
	
}; // color_label


} // color
} // editor
