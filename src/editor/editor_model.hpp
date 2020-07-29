#pragma once

#include <QObject>
#include <QPair>
#include <QImage>
#include <QRegion>
#include <QPainter>
#include <QSet>
#include <QList>

namespace editor {

class editor_model {
public:
	enum painting_mode {over, replace};
	struct mask_pair : public QPair<QImage, painting_mode> {
		mask_pair(QImage image, painting_mode mode = over)
		: QPair<QImage, painting_mode>(image, mode) {};
	};
	
	enum select_type {select, exclude, remove};
	typedef QPair<QRegion, select_type> select_region;
	
	const QImage image();
	const QRect image_rect();
	const QRegion selected_region();
	const QRect region_rect();
	const QSet<QRgb> color_table();
	bool is_altered();
	
	void set_image(const QImage &new_image);
	void set_mask(const mask_pair& pair);
	void add_region(const select_region &region_pair);
	void clear_regions();
	
	const QImage apply_mask();
	void paint_on(QPainter *painter, const QBrush &background = Qt::NoBrush);
	
private:
	QImage _image;
	QSet<QRgb> _color_table;
	bool image_altered = false;
	
	QImage mask;
	painting_mode mode;
	
	QRegion selected;
	QRegion excluded;
	QRegion combined;
	QList<select_region> region_changes;
	
	void update_color_table();
	
}; // editor_model

} // editor

Q_DECLARE_METATYPE(editor::editor_model::painting_mode);
Q_DECLARE_METATYPE(editor::editor_model::select_type);
