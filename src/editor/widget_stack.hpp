#pragma once

#include <QStackedWidget>
#include <QComboBox>
#include <QVBoxLayout>

namespace editor {

/* a convenience class for a QStackedWidget with a QComboBox above it
 * the main convenience is the automatic casting between the QWidget* and T
 * when calling stack->currentWidget()
 * has support for range based for loops i.e. for (auto widg : widg_stack) {}
 * you can also just do: for (int i=0; i < size(); ++i) {} and access with at()
 * requirements for T:
 * - inherits from QWidget
 * - is a pointer to a type, not the actual type
 * - has the public name() function, used inside the combo box
 * 
 * Qt doens't support full QObject functionality for templated types, but dammmit
 * I use this concept twice and there's no way I'm gonna write two copies of the
 * same exact code
 * ugh
 * anyways, that's the reason I can't emit signals when the active selector is 
 * changed. I've thus used hide events for color effects and button groups and 
 * passing of focus for selectors to do effectively the same thing
 */
template <typename T>
class widget_stack : public QWidget {
public:
	// initializes members and connects the combo box to the stacked widget
	// also sets the layout: the combobox is above the stack in a simple vbox
	widget_stack(QWidget *parent = nullptr) : QWidget(parent) {
		stack = new QStackedWidget;
		box = new QComboBox;
		
		connect(box, QOverload<int>::of(&QComboBox::currentIndexChanged),
				stack, &QStackedWidget::setCurrentIndex);
		
		auto vbox = new QVBoxLayout;
		vbox->setContentsMargins(0, 0, 0, 0);
		vbox->addWidget(box);
		vbox->addWidget(stack);
		setLayout(vbox);
	}
	
	// add a new T to the stack and the box
	void add(T new_item, const QString &name) {
		// since items get added at the same time, they have the same index
		// in the stack and the box
		stack->addWidget(new_item);
		box->addItem(name);
	}
	
	// add a new item and deduce its name using a public name() function
	void add(T new_item) {
		add(new_item, new_item->name());
	}
	
	// return the active item
	T active() const {
		return qobject_cast<T>(stack->currentWidget());
	}
	
	// return the item at index
	T at(int index) const {
		return qobject_cast<T>(stack->widget(index));
	}
	
	// return the number of items in the widget_stack
	int size() const {
		return stack->count();
	}
	
	// adds support for range based for loops
	// really just a wrapper around an int, very basic
	class Iterator {
	public:
		Iterator(const widget_stack<T> *parent_in, int position_in)
		: parent(parent_in), position(position_in) {}
		
		T operator*() const {
			return parent->at(position);
		}
		Iterator &operator++() {
			++position;
			return *this;
		}
		bool operator==(const Iterator &rhs) const {
			return parent == rhs.parent && position == rhs.position;
		}
		bool operator!=(const Iterator &rhs) const {
			return !(*this == rhs);
		}
		
	private:
		const widget_stack<T> *parent;
		int position;
	}; // Iterator
	
	// 0
	Iterator begin() const {
		return Iterator(this, 0);
	}
	
	// size()
	Iterator end() const {
		return Iterator(this, size());
	}
	
protected:
	/* the stack handles the painting of the widgets, and the combobox handles
	 * selecting which one you want, they have a one-way connection, so the only
	 * way to change them is via the combobox
	 */
	QStackedWidget *stack;
	QComboBox *box;
	
}; // widget_stack

} // editor
