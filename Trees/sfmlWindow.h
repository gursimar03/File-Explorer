// sfmlWindow.h

#pragma once
#include <SFML/Graphics.hpp>
#include <iostream>
#include <vector>
#include "Tree.h"
#include "TreeIterator.h"

using namespace std;
using namespace sf;

// Reference: https://www.sfml-dev.org/tutorials/2.5/graphics-shape.php
// https://www.sfml-dev.org/tutorials/2.5/graphics-vertex-array.php
// https://www.sfml-dev.org/tutorials/2.5/graphics-text.php
// https://cplusplus.com/reference/string/string/npos/


class sfmlWindow {
public:
    // Constructor
    sfmlWindow(TreeIterator<string> itr);

    // Function to run the SFML window
    void run();

    // Test function
    void test();

private:
    // SFML window
    sf::RenderWindow window;

    TreeIterator<string> itr;

    TreeIterator<string> rootIter;

    Tree<string>* clickedFolder;


    Tree<string>* selectedFolder;

    // Font for text
    sf::Font font;

    sf::Font boldFont;

    // Text displaying the current folder name
    sf::Text currentFolderText;

    sf::Text upButton;

    sf::Text pruneFolderButton;

    sf::RectangleShape rectangle;

    sf::RectangleShape topBar;

    // Texts representing files/folders in the current folder
    std::vector<sf::Text> fileTexts;

    // Current folder path (using a string for now)
    std::string currentFolderPath;

    // Function to handle events
    void handleEvents();

    // Function to load files/folders in the current folder
    void loadCurrentFolder();

    void handleFolderClick(string folderName);

    void renderWindow();

    void handleUpButtonClick();

    void sfmlWindow::pruneTree(Tree<string>* iter);

    void sfmlWindow::handleSingleFolderClick(string folderName);

};


sfmlWindow::sfmlWindow(TreeIterator<string> itr) : window(sf::VideoMode(1000, 800), "SFML File Manager"), rootIter(itr) , itr(itr) {

    // Load Arial.ttf font from the same folder as the project
    if (!font.loadFromFile("Arial.ttf")) {
        // Handle font loading error
        std::cerr << "Failed to load font!" << std::endl;
    }

    if (!boldFont.loadFromFile("Arial Bold.ttf")) {
        // Handle font loading error
        std::cerr << "Failed to load font!" << std::endl;
    }

    rectangle.setSize(sf::Vector2f(00.f, 00.f));

    // Set initial current folder path to a sample path
    currentFolderPath = itr.node->name;
    
    topBar.setSize(sf::Vector2f(1000.f, 35.f));  // Set width and height as needed
    topBar.setFillColor(sf::Color(192, 192, 192));  // Silver color
    topBar.setPosition(0.f, 0.f);

    // Create text for displaying the current folder path on top bar
    currentFolderText.setFont(font);
    currentFolderText.setFillColor(sf::Color::Black);
    currentFolderText.setCharacterSize(16);
    currentFolderText.setString("Path: " + currentFolderPath);
    currentFolderText.setPosition(5.f, 10.f);

    // Initialize up button
    upButton.setFont(font);
    upButton.setCharacterSize(16);
    upButton.setString("Up");
    upButton.setFillColor(sf::Color::Blue);  // Customize the color
    upButton.setPosition(950.f, 10.f);  // Adjust the position as needed

    // Initialize up button
    pruneFolderButton.setFont(font);
    pruneFolderButton.setCharacterSize(20);
    pruneFolderButton.setString("Prune Folder");
    pruneFolderButton.setFillColor(sf::Color::Blue);  // Customize the color
    pruneFolderButton.setPosition(10.f, 750.f);  // Adjust the position as needed

	// Load files/folders in the current folder
    loadCurrentFolder();
}

// Function to run the SFML window
void sfmlWindow::run() {
    while (window.isOpen()) {
        handleEvents();
        renderWindow();
    }
}

// Function to handle events
void sfmlWindow::handleEvents() {
    static sf::Clock clock;
    static bool clickedOnce = false;

    sf::Event event;
    while (window.pollEvent(event)) {
        if (event.type == sf::Event::Closed) {
            window.close();
        }
        else if (event.type == sf::Event::MouseButtonPressed && event.mouseButton.button == sf::Mouse::Left) {
            // Check if the Up button was clicked
            if (upButton.getGlobalBounds().contains(sf::Vector2f(event.mouseButton.x, event.mouseButton.y))) {
                // Handle the Up button click
                handleUpButtonClick();
            }
            else if (pruneFolderButton.getGlobalBounds().contains(sf::Vector2f(event.mouseButton.x, event.mouseButton.y))) {
                // Handle the Prune Folder button click
               
                pruneTree(selectedFolder);
                loadCurrentFolder(); // Reload the current folder
                renderWindow();
            }

            // Check if any folder text was clicked
            for (const auto& fileText : fileTexts) {
                if (fileText.getGlobalBounds().contains(sf::Vector2f(event.mouseButton.x, event.mouseButton.y))) {
                    // Check for double-click
                    if (clock.getElapsedTime().asMilliseconds() < 500 && clickedOnce) {
                        // Double-click action
                        
                        // Add your double-click action here, like entering the folder
                        handleFolderClick(fileText.getString().toAnsiString());
                        loadCurrentFolder();
                        renderWindow();
                    }
                    else {
                        // Single-click action
                      
                        // add rectangle around the clicked text
                        rectangle.setSize(sf::Vector2f(fileText.getGlobalBounds().width + 750.f, fileText.getGlobalBounds().height));
                        rectangle.setPosition(fileText.getGlobalBounds().left, fileText.getGlobalBounds().top);
                        rectangle.setFillColor(sf::Color::Transparent);
                        rectangle.setOutlineThickness(2.f);
                        rectangle.setOutlineColor(sf::Color::Red);

                        handleSingleFolderClick(fileText.getString().toAnsiString());
                        renderWindow();
                    }

                    // Reset the clock and flag for the next click
                    clock.restart();
                    clickedOnce = !clickedOnce;
                }
            }
        }
    }
}




// Function to handle Up button click
void sfmlWindow::handleUpButtonClick() {
    if (itr.node->parent) {
        itr.up();  // Move up in the tree
        loadCurrentFolder();
        renderWindow();
    }
}

// Function to handle folder clicks
void sfmlWindow::handleFolderClick(string folderName) {
    // Set the clicked folder to be used in the next loadCurrentFolder call
    if (folderName == "")
    {
        return;
    }
    else {
        DListIterator<Tree<string>*> iter = itr.node->children->getIterator();

        while (iter.isValid())
        {
            cout << iter.item()->name << endl;
            if (iter.item()->name == folderName)
            {
                clickedFolder = iter.item();
            }
            iter.advance();
        }

    }

    if (!clickedFolder) {
        cerr << "Couldnt Find the folder" << endl;
    }


}


// Function to handle folder clicks
void sfmlWindow::handleSingleFolderClick(string folderName) {
    // Set the clicked folder to be used in the next loadCurrentFolder call
    if (folderName == "")
    {
        return;
    }
    else {
        DListIterator<Tree<string>*> iter = itr.node->children->getIterator();

        while (iter.isValid())
        {
            cout << iter.item()->name << endl;
            if (iter.item()->name == folderName)
            {
                selectedFolder = iter.item();
            }
            iter.advance();
        }

    }

    if (!clickedFolder) {
        cerr << "Couldnt Find the folder" << endl;
    }

}
// Function to load files/folders in the current folder
void sfmlWindow::loadCurrentFolder() {
    // Clear existing file/folder texts
    fileTexts.clear();
    // Clear the rectangle
    rectangle.setSize(sf::Vector2f(0.f, 0.f));
    string path = rootIter.node->name;

    // Check if a folder is clicked
    if (clickedFolder) {
        // Use the clicked folder as the current folder
        itr.node = clickedFolder;
        clickedFolder = nullptr;  // Reset the clickedFolder flag
        path = rootIter.node->pathTo(itr.node->name);
    }

    // Update current folder text
    currentFolderText.setString("Path: " + path);
    currentFolderText.setFont(font);
    currentFolderText.setCharacterSize(16);
    currentFolderText.setPosition(10.f, 10.f); // Adjust the position as needed

    // Create header text for columns
    sf::Text headerName, headerType, headerSize, headerItems, headerMemoryUsage;
    headerName.setFont(boldFont);
    headerType.setFont(boldFont);
    headerSize.setFont(boldFont);
    headerItems.setFont(boldFont);
    headerMemoryUsage.setFont(boldFont);
    headerName.setCharacterSize(18);
    headerType.setCharacterSize(18);
    headerSize.setCharacterSize(18);
    headerItems.setCharacterSize(18);
    headerMemoryUsage.setCharacterSize(18);
    headerName.setString("Name");
    headerType.setString("Type");
    headerSize.setString("Size");
    headerItems.setString("Items");
    headerMemoryUsage.setString("Memory Usage");
    headerName.setPosition(10.f, 40.f);
    headerType.setPosition(350.f, 40.f);
    headerSize.setPosition(550.f, 40.f);
    headerItems.setPosition(700.f, 40.f);
    headerMemoryUsage.setPosition(850.f, 40.f);  // Adjust the position as needed
    fileTexts.push_back(headerName);
    fileTexts.push_back(headerType);
    fileTexts.push_back(headerSize);
    fileTexts.push_back(headerItems);
    fileTexts.push_back(headerMemoryUsage);

    // Set vertical gap between file and folder text
    float verticalGap = 10.f;

    // Iterate over the immediate children of the current folder
    DListIterator<Tree<string>*> childIter = itr.node->children->getIterator();

    while (childIter.isValid()) {
        Tree<string>* currentNode = childIter.item();

        // Create text objects for each column
        sf::Text textName, textType, textSize, textItems, textMemoryUsage;
        textName.setFont(font);
        textType.setFont(font);
        textSize.setFont(font);
        textItems.setFont(font);
        textMemoryUsage.setFont(font);
        textName.setCharacterSize(16);
        textType.setCharacterSize(16);
        textSize.setCharacterSize(16);
        textItems.setCharacterSize(16);
        textMemoryUsage.setCharacterSize(16);

        // Set the name
        string displayName = currentNode->name;
        textName.setString(displayName);

        // Set color based on the node type
        if (currentNode->data == "dir") {
            textName.setFillColor(sf::Color::Yellow);  // Color for folders
            textType.setString("Folder");
            textSize.setString("");
        }
        else if (currentNode->data == "file") {
            textName.setFillColor(sf::Color::White);  // Color for files
            textType.setString(currentNode->type);
            textSize.setString(to_string(currentNode->length) + " bytes");
        }

        // Set number of items for directories
        if (currentNode->data == "dir") {
            int itemCount = currentNode->count();
            textName.setFont(boldFont);
            textItems.setString(to_string(itemCount - 1) + " items");
        }
        else {
            textItems.setString("");  // No items for files
        }

        // Set memory usage for directories
        if (currentNode->data == "dir") {
            // Call the memoryUsage function to calculate memory usage
            int memory = currentNode->memoryUsage(currentNode);
            textMemoryUsage.setString(to_string(memory) + " KB");
        }
        else {
            textMemoryUsage.setString("");  // No memory usage for files
        }

        // Set positions for each column
        textName.setPosition(10.f, 70.f + fileTexts.size() * (verticalGap));
        textType.setPosition(350.f, 70.f + fileTexts.size() * (verticalGap));
        textSize.setPosition(550.f, 70.f + fileTexts.size() * (verticalGap));
        textItems.setPosition(700.f, 70.f + fileTexts.size() * (verticalGap));
        textMemoryUsage.setPosition(850.f, 70.f + fileTexts.size() * (verticalGap));  // Adjust the position as needed

        // Add the text to fileTexts
        fileTexts.push_back(textName);
        fileTexts.push_back(textType);
        fileTexts.push_back(textSize);
        fileTexts.push_back(textItems);
        fileTexts.push_back(textMemoryUsage);

        // Advance to the next child
        childIter.advance();
    }
}

void sfmlWindow::pruneTree(Tree<string>* node)
{

    if (node->children->count == 0) {
        return;
    }

    DListIterator<Tree<string>*> iter = node->children->getIterator();

    while (iter.isValid())
    {
        Tree<string>* child = iter.item();


        // Check if memoryUsage is 0 and it's a directory
        if ((child->memoryUsage(child) == 0) && (child->getData() == "dir"))
        {
            iter = node->children->remove(iter);
        }
        else
        {
            pruneTree(child);
            if (iter.isValid()) {
                iter.advance();
            }
        }
    }
}



// Function to render the window
void sfmlWindow::renderWindow() {
    // Clear the window
    window.clear();

    // Render the top bar
    window.draw(topBar);

    // Render the current folder path on top bar
    window.draw(currentFolderText);

    // Render the Up button
    window.draw(upButton);


    // Render the button
    window.draw(pruneFolderButton);

    // Render the file/folder texts
    for (const sf::Text& text : fileTexts) {
        window.draw(text);
    }
    window.draw(rectangle);
    // Display the contents of the window
    window.display();
}

// Test function
void sfmlWindow::test() {
    run();
}
