#pragma once

#include <QStackedWidget>
#include <QComboBox>
#include <QVBoxLayout>

namespace editor {

// a convenience class for a QStackedWidget with a QComboBox above it
// requires that T has public name() function and inherits from QWidget
// the value of name() is the identifier given to the combo box
// you can loop through all items in the stack by using count() and at(index)
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
	void add(T *new_item) {
		stack->addWidget(new_item);
		box->addItem(new_item->name());
	}
	
	// return the active item
	T *active() {
		return qobject_cast<T *>(stack->currentWidget());
	}
	
	// return the item at index
	T *at(int index) {
		return qobject_cast<T *>(stack->widget(index));
	}
	
	// return the number of items in the widget_stack
	int count() {
		return stack->count();
	}
	
private:
	QStackedWidget *stack;
	QComboBox *box;
	
}; // widget_stack

} // editor
