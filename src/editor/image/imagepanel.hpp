#pragma once

#include "./imagearea.hpp"

#include <QWidget>
#include <QString>

namespace editor{
namespace image {

class image_panel : public QWidget {
	Q_OBJECT
	
public:
	image_panel(QWidget* parent);
	
private:
	image_area *img_area;
	
}; // image_panel

} // image
} // editor
