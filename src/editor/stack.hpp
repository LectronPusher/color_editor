#pragma once

#include <forward_list>
#include <stack>

namespace editor {

// forward declaration, see below stack
template <typename T> class backward_list;

/* 
 * this is a very basic reimplementation of std::stack, but with clear()
 * it's really nothing more, really.
 * by default it uses backward list for low overhead, but I included 
 * backward_list_size if you want another easy option that has size()
 */ 
template <typename T, typename container = backward_list<T> >
class stack : public std::stack<T, container> {
public:
	void clear() {
		this->c.clear();
	}
	// just so we're clear, here's what the stack interface looks like:
	using std::stack<T, container>::top;
	using std::stack<T, container>::pop;
	using std::stack<T, container>::push;
	using std::stack<T, container>::size; // depends on container
}; // stack


/* 
 * std::forward_list is the lowest overhead list in the stl
 * but it doesn't have back(), push_back(), or pop_back() because it's a
 * forward* list. 
 * well, here's a backwards list that does have those functions
 * 
 * it's really just calling the forward_list functions by new names, nothing more
 */
template <typename T>
class backward_list : public std::forward_list<T> {
public:
	T &back() {
		return std::forward_list<T>::front();
	}
	void pop_back() {
		std::forward_list<T>::pop_front();
	}
	void push_back(const T &value) {
		std::forward_list<T>::push_front(value);
	}
	void push_back(T &&value) {
		std::forward_list<T>::push_front(std::move(value));
	}
	void clear() {
		std::forward_list<T>::clear();
	}
private:
	using std::forward_list<T>::front;
	using std::forward_list<T>::pop_front;
	using std::forward_list<T>::push_front;
}; // backward_list


// just a bit of overhead if you need an easy size function in your stack
template <typename T>
class backward_list_size : public std::forward_list<T> {
public:
	T &back() {
		return std::forward_list<T>::front();
	}
	void pop_back() {
		std::forward_list<T>::pop_front();
		--num_elements;
	}
	void push_back(const T &value) {
		std::forward_list<T>::push_front(value);
		++num_elements;
	}
	void push_back(T &&value) {
		std::forward_list<T>::push_front(std::move(value));
		++num_elements;
	}
	void clear() {
		std::forward_list<T>::clear();
		num_elements = 0;
	}
	size_t size() const {
		return num_elements;
	}
private:
	size_t num_elements = 0;
	using std::forward_list<T>::front;
	using std::forward_list<T>::pop_front;
	using std::forward_list<T>::push_front;
}; // backward_list

}; // editor
