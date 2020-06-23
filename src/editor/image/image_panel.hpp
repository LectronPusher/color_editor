#pragma once

#include "./image_base.hpp"

#include <QWidget>
#include <QString>
#include <QGraphicsScene>

namespace editor{
namespace image {

class image_panel : public QWidget {
	Q_OBJECT
	
public:
	image_panel(QWidget* parent);
	
private:
// 	bool valid_image = false;
	QGraphicsScene *scene;
	
}; // image_panel

} // image
} // editor
