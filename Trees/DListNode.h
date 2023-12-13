#pragma once

template <class T>
class DListNode
{
public:
	T data;
	DListNode<T> *next;
	DListNode<T> *previous;

	DListNode(T item);
	DListNode(DListNode<T> &item);
	void insertBefore(T item);
	void insertAfter(T item);

	~DListNode()
	{
	}
};


template <class T>
DListNode<T>::DListNode(DListNode<T> &node)
{
	this->data = new DListNode(node->data);
}
template <class T>
DListNode<T>::DListNode(T item)
{
	data = item;
	next = previous = nullptr;
}

template <class T>
void DListNode<T>::insertAfter(T item)
{
	DListNode<T> *temp = new DListNode(item);
	temp->previous = this;
	temp->next = this->next;
	if (this->next != nullptr)
	{
		this->next->previous = temp;
	}
	this->next = temp;
}



template <class T>
void DListNode<T>::insertBefore(T item)
{
	DListNode<T> *temp = new DListNode(item);
	temp->next = this;
	temp->previous = this->previous;
	if (this->previous != nullptr)
	{
		this->previous->next = temp;
	}
	this->previous = temp;
}
