#pragma once
#include "Tree.h"

template <class T>
class TreeIterator
{
public:
	// Data members
	Tree<T>* node;
	DListIterator<Tree<T>*> childIter;

	// Constructor
	TreeIterator(Tree<T>* root);

	// Reset the iterator to the beginning
	void resetIterator();

	// Move to the root of the tree
	void root();

	// Move to the parent of the current node
	void up();

	// Move to the first child of the current node
	void down();

	// Move to the previous child of the current node
	void childBack();

	// Move to the next child of the current node
	void childForth();

	// Move to the start of the children list of the current node
	void childStart();

	// Move to the end of the children list of the current node
	void childEnd();

	// Append a new child with the given item to the current node
	void appendChild(T item);

	// Prepend a new child with the given item to the current node
	void prependChild(T item);

	// Insert a new child with the given item before the current child
	void insertChildBefore(T item);

	// Insert a new child with the given item after the current child
	void insertChildAfter(T item);

	// Remove the current child
	void removeChild();

	// Check if the current child is valid
	bool childValid();

	// Get the data of the current child
	T childItem();

	// Get the data of the current node
	T item();
};

// Constructor
template <class T>
TreeIterator<T>::TreeIterator(Tree<T>* root)
{
	node = root;
	resetIterator();
}

// Reset the iterator to the beginning
template <class T>
void TreeIterator<T>::resetIterator()
{
	if (node != nullptr)
	{
		childIter = node->children->getIterator();
	}
	else
	{
		childIter = nullptr;
	}
}

// Move to the root of the tree
template <class T>
void TreeIterator<T>::root()
{
	if (node->parent != nullptr)
	{
		node = node->parent;
		root();
	}
	resetIterator();
}

// Move to the parent of the current node
template <class T>
void TreeIterator<T>::up()
{
	if (node->parent != nullptr)
	{
		node = node->parent;
	}
	resetIterator();
}

// Move to the first child of the current node
template <class T>
void TreeIterator<T>::down()
{
	if (childIter.isValid())
	{
		node = childIter.item();
	}
	resetIterator();
}

// Move to the previous child of the current node
template <class T>
void TreeIterator<T>::childBack()
{
	if (childIter.isValid())
	{
		childIter.previous();
	}
}

// Move to the next child of the current node
template <class T>
void TreeIterator<T>::childForth()
{
	if (childIter.isValid())
	{
		childIter.advance();
	}
}

// Move to the start of the children list of the current node
template <class T>
void TreeIterator<T>::childStart()
{
	if (childIter.isValid())
	{
		childIter.start();
	}
}

// Move to the end of the children list of the current node
template <class T>
void TreeIterator<T>::childEnd()
{
	if (childIter.isValid())
	{
		childIter.end();
	}
}

// Append a new child with the given item to the current node
template <class T>
void TreeIterator<T>::appendChild(T item)
{
	Tree<T>* temp = new Tree<T>(item);
	temp->parent = node;
	node->children->append(temp);
	resetIterator();
}

// Prepend a new child with the given item to the current node
template <class T>
void TreeIterator<T>::prependChild(T item)
{
	Tree<T>* temp = new Tree<T>(item);
	temp->parent = node;
	node->children->prepend(temp);
	resetIterator();
}

// Insert a new child with the given item before the current child
template <class T>
void TreeIterator<T>::insertChildBefore(T item)
{
	Tree<T>* temp = new Tree<T>(item);
	temp->parent = node;
	node->children->insert(childIter, temp);
}

// Insert a new child with the given item after the current child
template <class T>
void TreeIterator<T>::insertChildAfter(T item)
{
	Tree<T>* temp = new Tree<T>(item);
	temp->parent = node;
	childIter.advance();
	node->children->insert(childIter, temp);
	childIter.previous();
	childIter.previous();
}

// Remove the current child
template <class T>
void TreeIterator<T>::removeChild()
{
	childIter = node->children->remove(childIter);
}

// Check if the current child is valid
template <class T>
bool TreeIterator<T>::childValid()
{
	return childIter.isValid();
}

// Get the data of the current child
template <class T>
T TreeIterator<T>::childItem()
{
	return childIter->node->data;
}

// Get the data of the current node
template <class T>
T TreeIterator<T>::item()
{
	return node->data;
}
