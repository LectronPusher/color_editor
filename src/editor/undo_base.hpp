#pragma once

#include "stack.hpp"

namespace editor {

// a convenience class for implementing undo features
// override apply and unapply, and you'll have the functions working as expected
// the stacks are protected for custom use if necessary
template <typename T>
class undo_base {
public:
	void add(T&& value) {
		past_changes.push(value);
		future_changes.clear();
		apply(past_changes.top());
	}
	bool undo() {
		if (past_changes.empty()) return false;
		future_changes.push(std::move(past_changes.top()));
		past_changes.pop();
		unapply(future_changes.top());
		return true;
	}
	bool redo() {
		if (future_changes.empty()) return false;
		past_changes.push(std::move(future_changes.top()));
		future_changes.pop();
		apply(past_changes.top());
		return true;
	}
	
protected:
	virtual void apply(const T&) = 0;
	virtual void unapply(const T&) = 0;
	
	stack<T> past_changes;
	stack<T> future_changes;
	
}; // undo_manager

} // editor
