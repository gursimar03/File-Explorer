#include "stdafx.h"
#include "CppUnitTest.h"
#include "../Trees/Tree.h"

// After adding SFML properties to the project I was not able to run any tests anymore. all the tests here were working before.
// I tried to fix it but I was not able to. I hope you can still see the code and the tests I wrote.



//Refrences : https://cplusplus.com/reference/streambuf/streambuf/
//Refrences : https://stackoverflow.com/questions/421573/best-way-to-extract-a-subvector-from-a-vector

using namespace Microsoft::VisualStudio::CppUnitTestFramework;
using namespace std;

namespace treeTest
{
	TEST_CLASS(treeTest)
	{
	public:

		// Test tree creation and getData method
		TEST_METHOD(TreeCreationAndGetData)
		{
			Tree<int> tree(42);
			Assert::AreEqual(tree.getData(), 42);
		}

		// Test count method for an empty tree
		TEST_METHOD(CountEmptyTree)
		{
			Tree<int> tree(10);
			Assert::AreEqual(tree.count(), 1);
		}

		// Test count method for a tree with one child
		TEST_METHOD(CountTreeWithOneChild)
		{
			Tree<int> tree(5);
			Tree<int>* child = new Tree<int>(3);
			tree.children->append(child);
			Assert::AreEqual(tree.count(), 2);
		}

		// Test memoryUsage method for an empty tree
		TEST_METHOD(MemoryUsageEmptyTree)
		{
			Tree<int> tree(10);
			Assert::AreEqual(tree.memoryUsage(&tree), 0);
		}

		// Test memoryUsage method for a tree with one child
		TEST_METHOD(MemoryUsageTreeWithOneChild)
		{
			Tree<int> tree(5);
			Tree<int>* child = new Tree<int>(3);
			child->setLength(15);
			tree.children->append(child);
			Assert::AreEqual(tree.memoryUsage(&tree), 15);
		}

		// Test getNodeType method for a folder
		TEST_METHOD(GetNodeTypeForFolder)
		{
			Tree<string> folder("folder");
			folder.setType("dir");
			Assert::AreEqual(folder.getNodeType(), string("FOLDER"));
		}

		// Test getNodeType method for a file
		TEST_METHOD(GetNodeTypeForFile)
		{
			Tree<string> file("file");
			file.setType("file");
			Assert::AreEqual(file.getNodeType(), string("FILE"));
		}

		// Test getNodeType method for an unknown type
		TEST_METHOD(GetNodeTypeForUnknownType)
		{
			Tree<string> unknown("unknown");
			Assert::AreEqual(unknown.getNodeType(), string("UNKNOWN"));
		}

		// Test pathTo method for a tree with a specific name
		TEST_METHOD(PathToNameInTree)
		{
			Tree<string> tree("root");
			Tree<string>* child = new Tree<string>("folder");
			tree.children->append(child);
			Assert::AreEqual(tree.pathTo("folder"), string("root/folder"));
		}

		// Test displayContents method for an empty tree
		TEST_METHOD(DisplayContentsEmptyTree)
		{
			Tree<int> tree(10);
			stringstream buffer;
			streambuf* old = cout.rdbuf(buffer.rdbuf()); // Redirect cout to buffer
			tree.displayContents("folder");
			cout.rdbuf(old); // Restore cout
			Assert::AreEqual(buffer.str(), string(""));
		}

		// Test displayContents method for a tree with a specific folder name
		TEST_METHOD(DisplayContentsWithSpecificFolder)
		{
			Tree<string> tree("root");
			Tree<string>* folder = new Tree<string>("folder");
			Tree<string>* file = new Tree<string>("file");
			folder->setType("dir");
			file->setType("file");
			tree.children->append(folder);
			tree.children->append(file);

			stringstream buffer;
			streambuf* old = cout.rdbuf(buffer.rdbuf()); // Redirect cout to buffer
			tree.displayContents("folder");
			cout.rdbuf(old); // Restore cout
			Assert::AreEqual(buffer.str(), string("root/folder\n"));
		}


		// Test extractInteger method for a string with an integer
		TEST_METHOD(ExtractIntegerFromStringWithInteger)
		{
			Tree<string> tree("root");
			tree.extractInteger("Length: 42 bytes");
			Assert::AreEqual(tree.getLength(), 42);
		}

		// Test extractInteger method for a string without an integer
		TEST_METHOD(ExtractIntegerFromStringWithoutInteger)
		{
			Tree<string> tree("root");
			tree.extractInteger("No integer in this string");
			Assert::AreEqual(tree.getLength(), 0); // Default value when no integer is found
		}

	};
}
