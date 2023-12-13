#pragma once
#include "DList.h"
#include "DListIterator.h"
#include <string>
#include <queue>
#include <iostream>

using namespace std;

template<class T>
class Tree
{
public:

	enum NodeType {
		FOLDER,
		FILE
	};

	// Data members
	T data;
	string name;
	string type;
	int length;

	NodeType nodeType;

	Tree<T>* parent;
	DList<Tree<T>*>* children;

	// Constructor
	Tree(T item);

	// Public methods
	int count();  // Count the number of nodes in the tree
	T getData();  // Get the data of the current node
	int memoryUsage(Tree<T>* node);  // Calculate memory usage of the tree
	string pathTo(string name);  // Get the path to a node with a specific name
	string getNodeType();  // Get the type of the current node (folder, file, etc.)
	void displayContents(string folderName);  // Display contents of the tree
	int extractInteger(const string& input);  // Extract an integer from a string

	// New methods
	void setLength(int newLength);  // Set the length of the current node
	int getLength();  // Get the length of the current node
	void setType(const string& newType);  // Set the type of the current node
	string getType();  // Get the type of the current node
};

// Method to set the length of the current node
template <class T>
void Tree<T>::setLength(int newLength)
{
	length = newLength;
}

// Method to get the length of the current node
template <class T>
int Tree<T>::getLength()
{
	return length;
}

// Method to set the type of the current node
template <class T>
void Tree<T>::setType(const string& newType)
{
	type = newType;
}

// Method to get the type of the current node
template <class T>
string Tree<T>::getType()
{
	return type;
}

// Method to get the data of the current node
template <class T>
T Tree<T>::getData()
{
	return data;
}

// Method to calculate the memory usage of the tree
template<class T>
int Tree<T>::memoryUsage(Tree<T>* node)
{
	int sum = 0;

	// Queue for BFS
	std::queue<Tree<T>*> bfsQueue;
	bfsQueue.push(node);

	while (!bfsQueue.empty()) {
		Tree<T>* current = bfsQueue.front();
		bfsQueue.pop();

		// Add the length of the current node if it has the "length" attribute
		sum += current->length;

		// Enqueue child nodes for further exploration
		DListIterator<Tree<T>*> iter = current->children->getIterator();
		while (iter.isValid()) {
			bfsQueue.push(iter.item());
			iter.advance();
		}
	}

	return sum;
}

// Method to get the path to a node with a specific name
template <class T>
string Tree<T>::pathTo(string name)
{
	if (name == "") {
		return "";
	}

	string path = "";
	bool found = false;
	if (this->name.find(name) != string::npos)
	{
		path = this->name;
		return path;
	}
	else
	{
		DListIterator<Tree<T>*> iter = children->getIterator();
		while (iter.isValid())
		{
			path = iter.item()->pathTo(name);
			if (path != "")
			{
				path = this->name + "/" + path;
				return path;
			}
			iter.advance();
		}
	}
	return path;
}

// Method to display contents of the tree
template<class T>
void Tree<T>::displayContents(string folderName)
{
	if (folderName == "")
	{
		return;
	}
	else
	{
		if (this->name.find(folderName) != string::npos)
		{
			cout << this->name << endl;
		}
	}
	DListIterator<Tree<T>*> iter = children->getIterator();
	while (iter.isValid())
	{
		if (iter.item()->type == "dir")
		{
			cout << iter.item()->name << endl;
		}
		else if (iter.item()->type == "file")
		{
			cout << iter.item()->name << " " << iter.item()->length << endl;
		}
		iter.item()->displayContents(folderName);
		iter.advance();
	}
}

// Method to extract an integer from a string
template<class T>
inline int Tree<T>::extractInteger(const string& input)
{
	std::regex integerRegex("\\b(\\d+)\\b");
	std::smatch match;

	if (std::regex_search(input, match, integerRegex)) {
		length = stoi(match[1]);
	}
	else {
		// Handle the case where no integer is found in the string
		return 0;  // Or you can return some other default value
	}
}

// Method to get the type of the current node (folder, file, etc.)
template <class T>
string Tree<T>::getNodeType() {

	switch (nodeType) {
	case NodeType::FOLDER:
		return "FOLDER";
	case NodeType::FILE:
		return "FILE";
	default:
		return "UNKNOWN"; // Handle unexpected values
	}
}

// Constructor
template <class T>
Tree<T>::Tree(T item)
{
	data = item;
	children = new DList<Tree<T>*>();
	parent = nullptr;
}

// Method to count the number of nodes in the tree
template <class T>
int Tree<T>::count()
{
	int c = 1;
	DListIterator<Tree<T>*> iter = children->getIterator();
	while (iter.isValid())
	{
		c += iter.item()->count();
		iter.advance();
	}
	return c;
}
