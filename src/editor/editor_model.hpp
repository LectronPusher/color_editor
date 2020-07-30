#pragma once

#include <QObject>
#include <QPair>
#include <QImage>
#include <QRegion>
#include <QPainter>
#include <QSet>

#include <QStack>
#include <list>

namespace editor {

class editor_model : public QObject {
	Q_OBJECT
	
public:
	enum painting_mode {over, replace};
	struct mask_pair : public QPair<QImage, painting_mode> {
		mask_pair(QImage image, painting_mode mode = over)
		: QPair<QImage, painting_mode>(image, mode) {};
	};
	
	enum select_type {select, exclude, remove, clear};
	typedef QPair<QRegion, select_type> select_region;
	typedef QPair<QRegion, QRegion> region_pair; // selected first, excluded second
	
private:
	struct undo_data {
		undo_data(select_type type, QRegion region);
		undo_data(select_type type, region_pair regions);
		undo_data(const undo_data &other);
		undo_data operator=(const undo_data &other);
		~undo_data();
		
		select_type s_type;
		union {
			QRegion added_region;
			region_pair removed_regions;
		};
	}; // undo_data
	
public:
	const QImage image();
	const QRect image_rect();
	const QRegion selected_region();
	const QRect region_rect();
	const QSet<QRgb> color_table();
	bool is_altered();
	
	void set_image(const QImage &new_image);
	void set_mask(const mask_pair& pair);
	const QImage apply_mask();
	void paint_on(QPainter *painter, const QBrush &background = Qt::NoBrush);
	
	void add_region(const select_region &region_pair);
	void clear_regions();
	void undo();
	void redo();
	void combine_recent_undos(int n);
	
signals:
	void image_changed(QImage image);
	void contents_updated(QRect rect);
	void boundary_rect_updated(QRect rect);
	
private:
	QImage _image;
	QSet<QRgb> _color_table;
	
	QImage mask;
	painting_mode mode;
	
	QRegion selected;
	QRegion excluded;
	QRegion combined;
	
	QStack<undo_data> applied_changes;
	QStack<undo_data> undone_changes;
	
	void apply_change(const undo_data &data);
	void update_combined();
	void update_color_table();
	
}; // editor_model

} // editor

Q_DECLARE_METATYPE(editor::editor_model::painting_mode);
Q_DECLARE_METATYPE(editor::editor_model::select_type);
