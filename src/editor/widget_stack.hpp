#pragma once

#include <QStackedWidget>
#include <QComboBox>

namespace editor {

template <typename T>
class widget_stack : public QWidget {
public:
	widget_stack() {
		stack = new QStackedWidget;
		box = new QComboBox;
		
		auto box_changed = QOverload<int>::of(&QComboBox::currentIndexChanged);
		connect(box, box_changed, stack, &QStackedWidget::setCurrentIndex);
		
		stack->setSizePolicy(QSizePolicy(QSizePolicy::Maximum,
										 QSizePolicy::Maximum));
		auto vbox = new QVBoxLayout;
		vbox->setContentsMargins(0, 0, 0, 0);
		vbox->addWidget(box);
		vbox->addWidget(stack);
		setLayout(vbox);
	}
	
	void add(T *new_item) {
		stack->addWidget(new_item);
		box->addItem(new_item->name);
	}
	
	T *active() {
		return static_cast<T *>(stack->currentWidget());
	}
	
private:
	QStackedWidget *stack;
	QComboBox *box;
	
}; // widget_stack

} // editor
