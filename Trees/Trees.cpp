#include "Tree.h"
#include "TreeIterator.h"
#include <string>
#include <iostream>
#include <queue>
#include <fstream>
#include <stack>
#include <regex>
#include "sfmlWindow.h"


//References : https://www.geeksforgeeks.org/stack-in-cpp-stl/
//             https://www.geeksforgeeks.org/queue-cpp-stl/
//             Https://www.geeksforgeeks.org/regex-regular-expressions-in-c/
//doubly linked list : https://www.geeksforgeeks.org/doubly-linked-list/

using namespace std;

template<typename T>
Tree<T>* loadXML(const string& xmlFilename) {
   

    ifstream file(xmlFilename);
    if (file.is_open()) {
   
        Tree<T>* root = nullptr;
        Tree<T>* currentNode = nullptr;
        stack<Tree<T>*> nodeStack;
        string line;

        while (getline(file, line)) {
            size_t pos = 0;
            while ((pos = line.find('<', pos)) != string::npos) {
                size_t endPos = line.find('>', pos);
                if (endPos == string::npos) {
                    cerr << "Error: Invalid XML tag in line: " << line << endl;
                    return nullptr;
                }

                string tag = line.substr(pos + 1, endPos - pos - 1);

                if (tag[0] != '/') {
                    if (tag == "dir" || tag == "file") {
                        Tree<T>* newNode = new Tree<T>(tag);

                        

                        newNode->parent = currentNode;

                        if (currentNode) {
                            currentNode->children->append(newNode);
                        }
                        else {
                            root = newNode;
                        }

                        nodeStack.push(newNode);
                        currentNode = newNode;
                    }
                    else {
                        if (currentNode) {
                            string value = line.substr(endPos + 1, line.find('<', endPos) - endPos - 1);

                            
                            if (tag == "name") {
                                currentNode->name = value;
                            }
                            else if (tag == "length") {
                                currentNode->extractInteger(value);
                            }
                            else if (tag == "type") {
                                currentNode->type = value;
                            }
                        }
                    }
                }
                else {
                    if (!nodeStack.empty()) {
                        string closingTag = tag.substr(1);
                        currentNode = nodeStack.top();

                        if (closingTag == "dir" || closingTag == "file") {
                            if (closingTag != currentNode->getData()) {
                                cerr << "Error: Mismatched closing tag in line: " << line << endl;
                                return nullptr;
                            }
                            nodeStack.pop();
                            currentNode = currentNode->parent; // Move up the tree
                        }
                    }
                }
                pos = endPos + 1;
            }
        }
        return root;
    }
    else {
        cerr << "Error opening XML file: " << xmlFilename << endl;
        return nullptr;
    }
}

void printDFS(TreeIterator<string> iter)
{
    while (iter.childValid())
    {
        TreeIterator<string> iter2(iter.childIter.currentNode->data);
        printDFS(iter2);
        iter.childForth();
    }
    cout << iter.item() << ", ";
}
/*
    Print using a Breadth first Search
*/
void printBFS(Tree<string> tree)
{
    queue<Tree<string>> queue;
    queue.push(tree);
    while (!queue.empty())
    {
        DListIterator<Tree<string>*> iter = queue.front().children->getIterator();
        while (iter.isValid())
        {
            queue.push(*iter.item());
            iter.advance();
        }
        cout << queue.front().data << ", ";
        queue.pop();

    }


}




void displayTree(TreeIterator<string> iter, string indent)
{
    if (iter.node->data == "dir") {
        cout << indent << iter.node->getNodeType() << " " << iter.node->name;
    }
    else {
        cout << indent << iter.node->name << " " << iter.node->length << " " << iter.node->type;
    }

    if (iter.childValid())
    {
        cout << "" << endl;

        while (iter.childValid())
        {
            TreeIterator<string> iter2(iter.childIter.currentNode->data);
            displayTree(iter2, "\t" + indent);
            iter.childForth();
        }
        cout << indent;
    }
    cout << endl;
}



int main() {
    const string xmlFilename = "XMLFile.xml";
    Tree<string>* xmlTree = loadXML<string>(xmlFilename);
    TreeIterator<string> iter(xmlTree);

    if (xmlTree) {
        cout << "Welcome to the File Manager!\n";
        cout << "In the SFML window, double-click to enter a folder, single-click to select a folder,\n";
        cout << "and press the prune button to prune the selected folder.\n";
        cout << "Use the 'Up' button to go up a folder. You can see the number of items in the column\n";
        cout << "and also view the memory usage.\n";
        cout << "Note: Terminal functionality is limited as development continued directly to SFML.\n";

        int choice = 0;  // Initialize choice

        do {
            cout << "\nMenu:\n";
            cout << "1. Open SFML Window\n";
            cout << "2. Display Path to Node\n";
            cout << "3. Display Contents\n";
            cout << "4. Exit\n";
            cout << "Enter your choice: ";

            // Ensure the input is a valid integer
            while (!(cin >> choice)) {
                cin.clear();
                cin.ignore(numeric_limits<streamsize>::max(), '\n');
                cout << "Invalid input. Please enter a number: ";
            }

            switch (choice) {
            case 1:
            {
                sfmlWindow window(xmlTree);
                window.run();
            }
            break;
            case 2:
            {
                string nodeName;
                cout << "Enter the node name: ";
                cin >> nodeName;
                string path = iter.node->pathTo(nodeName);
                cout << "Path to Node: " << path << endl;
            }
            break;
            case 3:
            {
                string folderName;
                cout << "Enter the folder name: ";
                cin >> folderName;
                iter.node->displayContents(folderName);
            }
            break;
            case 4:
                cout << "Exiting...\n";
                break;
            default:
                cout << "Invalid choice. Please enter a number between 1 and 4.\n";
            }

        } while (choice != 4);
    }
    else {
        cout << "Failed to load or validate XML tree." << endl;
    }

    // Release
    delete xmlTree;

    return 0;
}