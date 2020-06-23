#pragma once

#include <QWidget>
#include <QPaintEvent>
#include <QImage>

namespace editor{
namespace image {

class image_area : public QWidget {
	Q_OBJECT
	
public:
	image_area(QWidget* parent);
	
	void load_image(QString filename);
	
	QSize sizeHint() const override;
	
protected:
	void paintEvent(QPaintEvent *) override;
	
private:
	QImage img;
	
}; // image_panel

} // image
} // editor
 
