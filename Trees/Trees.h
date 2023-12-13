// TreeOperations.h

#ifndef TREE_OPERATIONS_H
#define TREE_OPERATIONS_H

#include "Tree.h"
#include "TreeIterator.h"
#include <string>

// Function prototypes
template<typename T>
Tree<T>* loadXML(const std::string& xmlFilename);

void printDFS(TreeIterator<std::string> iter);
void printBFS(Tree<std::string> tree);
void displayTree(TreeIterator<std::string> iter, std::string indent);



#endif  // TREE_OPERATIONS_H
