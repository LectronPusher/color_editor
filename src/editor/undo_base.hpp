#pragma once

#include "stack.hpp"

namespace editor {

/*
 * usage guide:
 * - create a custom struct that stores undo information
 * - in your chosen class, inherit publicly from undo_base<custom_struct>
 * - in the class, override the apply and unapply functions (public or private)
 *   to do whatever needs to be done when you apply or unapply a change
 * - if you need to, make use of the past_changes and future_changes stacks to 
 *   to do custom undo work
 */

// TODO make (un)apply happen first and return a bool, if true, muck with stacks
// if false, don't muck with stacks

/* 
 * a convenience class for implementing undo features
 * override apply and unapply for a custom class, and you'll have undo and redo
 * working as expected
 * the past and future stacks are protected to allow custom use
 * 
 * note, undo_base moves the change items before it calls apply() or unapply()
 */
template <typename T>
class undo_base {
public:
	/* adds a change to the past changes, and clears all future changes
	 * then calls apply on the change
	 * this is how you initially add a change to the system, undo and redo just
	 * move between old states
	 */
	void add(T&& change) {
		past_changes.push(change);
		future_changes.clear();
		apply(past_changes.top());
	}
	// undoes a change by moving it from past to future, then calls unappply()
	bool undo() {
		if (past_changes.empty()) return false;
		future_changes.push(std::move(past_changes.top()));
		past_changes.pop();
		unapply(future_changes.top());
		return true;
	}
	// does a change by moving it from future to past, then calls appply()
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
