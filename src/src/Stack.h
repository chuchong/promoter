#pragma once
#include <assert.h>
//模板类,注意栈中元素为指针时外部需要delete
template<class T>
class Stack;

template<class T>
class Stack_Element {
private:
	T t_;
	Stack_Element * next_;
	Stack_Element(const T & t):t_(t),next_(0){
	}
	friend class Stack<T>;
	~Stack_Element() {
	}
};

template<class T>
class Stack
{
	Stack_Element<T> * head;
public:
	void push(const T & t);
	void pop();
	T top();
	bool empty();
	Stack();
	~Stack();
};

template<class T>
inline void Stack<T>::push(const T & t)
{
	Stack_Element<T> * ele = new Stack_Element<T>(t);
	if (head == 0) {
		head = ele;
	}
	else {
		ele->next_ = head;
		head = ele;
	}
}

template<class T>
inline void Stack<T>::pop()
{
	Stack_Element<T> * ele = head;
	assert(ele != 0);
	head = head->next_;
	delete ele;
}

template<class T>
inline T Stack<T>::top()
{
	return head->t_;
}

template<class T>
inline bool Stack<T>::empty()
{
	return head == nullptr;
}

template<class T>
inline Stack<T>::Stack():head(nullptr)
{
}

template<class T>
inline Stack<T>::~Stack()
{
	while (!empty()) {
		pop();
	}
}



