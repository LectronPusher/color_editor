#pragma once

#include <forward_list>
#include <stack>

namespace editor {

template <typename T> class backward_list;

// reimplementing std::stack but with a clear() function
template <typename T, typename container = backward_list<T> >
class stack : public std::stack<T, container> {
public:
	void clear() {
		this->c.clear();
	}
	// just so we're clear, here's what a stack interface looks like:
	using std::stack<T, container>::top;
	using std::stack<T, container>::pop;
	using std::stack<T, container>::push;
	using std::stack<T, container>::size; // depends on container
}; // stack


// std::forward_list is the lowest overhead list in the stl
// but it doesn't have back(), push_back(), or pop_back() because its a *forward* list
// well, here's a backwards one that does have that
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


// just a bit of overhead if you really need the size of the thing
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
