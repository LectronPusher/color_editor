#pragma once

#include "stack.hpp"

namespace editor {
;
// a convenience class for implementing undo features
// override apply and unapply, and you'll have the functions working as expected
// the stacks are private for custom use if necessary
template <typename T>
class undo_base {
public:
	void add(T&& value) {
		applied.push(value);
		undone.clear();
		apply(applied.top());
	}
	void undo() {
		if (applied.empty()) return;
		undone.push(std::move(applied.top()));
		applied.pop();
		unapply(undone.top());
	}
	void redo() {
		if (undone.empty()) return;
		applied.push(std::move(undone.top()));
		undone.pop();
		apply(applied.top());
	}
	
protected:
	virtual void apply(const T&) = 0;
	virtual void unapply(const T&) = 0;
	
	stack<T> applied;
	stack<T> undone;
	
}; // undo_manager

} // editor
