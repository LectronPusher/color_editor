# pragma once

#include <QFrame>
#include <QColor>
#include <QColorDialog>
#include <QPaintEvent>

namespace editor {
namespace color {


/* A small widget that displays a single color and allows the user to change it
 * with a QColorDialog that opens when the color_label is clicked on
 * 
 * inherits from QFrame to have a nice border, the border can be changed
 * 
 * defaults:
 * - starting color is white, change with a constructor argument
 * - doesn't allow transparent colors, change with a constructor argument
 * - allows interaction, change with a constructor argument
 * - framestyle of QFrame::Panel | QFrame::Sunken, change with setFrameStyle()
 * - size of 25x25 pixels, change with resize()
 * - size policy fixed/fixed, change with setSizePolicy
 */
class color_label : public QFrame {
	Q_OBJECT
	
public:
	// constructs the color_label with the defaults above
	color_label(QColor starting_color = Qt::white, bool allow_transparency = false,
				bool allow_interaction = true, QWidget *parent = nullptr);
	// returns the size, set to 25x25 in the constructor
	virtual QSize sizeHint() const override;
	// returns the color being displayed
	QColor color() const;
	
public slots:
	// set the current color and redraw the label if different
	// won't set the color if it isn't valid
	void set_color(const QColor &new_color);
	// allows setting the transparency without caring about the current color
	void set_alpha(int value);
	
signals:
	// emitted whenever the color changes
	void color_changed(const QColor &new_color);
	
protected:
	// these three all send the event to QFrame after being called
	// fills the painter with the fill color
	virtual void paintEvent(QPaintEvent *event) override;
	// closes the color dialog if open
	virtual void hideEvent(QHideEvent *event) override;
	// opens the dialog if interactive and there isn't a dialog
	// TODO make focus shift to the dialog if there is one
	virtual void mousePressEvent(QMouseEvent *event) override;
	
private:
	// the color of the dialog
	QColor fill_color;
	// whether the color_label can have a transparent color
	const bool can_be_transparent; // default false
	// whether clicking on the color_label will open a dialog
	const bool is_interactive; // default true
	
	// the opened color dialog, color_label will update the color live as the
	// dialog color is changed, not when the dialog is accepted or rejected
	QColorDialog *dialog = nullptr;
	
	// opens the dialog if there isn't one, connects a number of signals and slots
	// to allow for communication
	void open_dialog();
	
}; // color_label

} // color
} // editor
