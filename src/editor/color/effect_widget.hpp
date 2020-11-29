#pragma once

#include "effect_state.hpp"

#include <QWidget>
#include <QVBoxLayout>

namespace editor {
namespace color {

/* an effect_widget is an interface that allows a user to choose a color effect
 * by choose a color effect I mean create an effect_state matching the type of
 * the effect_widget
 * the GUI is stored in the obtions vbox, implementations will add their GUI
 * components to it
 * 
 * **this is an abstract class** that currently has three implementations found in
 * effect_types.hpp
 * 
 * effect_widgets emit altered whenever their state changes or on a show event
 * their state can be set with load_state and retrieved with get_state
 * 
 * name() will give a unique string set by each effect_widget implementation in
 * their constructors
 */
class effect_widget : public QWidget {
	Q_OBJECT
	
public:
	// sets effect_name to name_in, creates options, and calls setLayout(options)
	effect_widget(QString name_in);
	// the name of the effect_widget implementation
	const QString name() const;
	
	// sets GUI components to the given state
	virtual void load_state(effect_state &new_state) = 0;
	// returns the current state of the effect
	virtual effect_state get_state() = 0;
	
signals:
	// emitted whenever the state changes, or on a show event
	// mostly just connected to signals on implementation GUI components
	void altered(effect_state new_state);
	
protected:
	// a vbox to be used by implementations to store their GUI components in
	QVBoxLayout *options;
	
	// emits altered(get_state()) when shown, passes to QWidget::showEvent first
	void showEvent(QShowEvent *event) override;
	
private:
	const QString effect_name;
	
}; // effect_widget

} // color
} // editor
