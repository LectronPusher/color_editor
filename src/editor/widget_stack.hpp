#pragma once

#include <QStackedWidget>
#include <QComboBox>
#include <QVBoxLayout>

namespace editor {

// a convenience class for a QStackedWidget with a QComboBox above it
// requires that T has public name() function and inherits from QWidget
// the value of name() is the identifier given to the combo box
// supports range based for loops with widget_stack<T>::Iterator
template <typename T>
class widget_stack : public QWidget {
public:
	// setup the stack and connect the combo box to the stacked widget
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
	void add(T new_item) {
		stack->addWidget(new_item);
		box->addItem(new_item->name());
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
	
	// support range based for loops
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
	
	Iterator begin() const {
		return Iterator(this, 0);
	}
	
	Iterator end() const {
		return Iterator(this, size());
	}
	
private:
	QStackedWidget *stack;
	QComboBox *box;
	
}; // widget_stack

} // editor
