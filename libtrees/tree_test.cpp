#include "tree.hpp"
#include <iostream>
#include <string>

void Test(bool expr, unsigned int test_no)
{
    if(expr)
    {
        std::cout << "✔️ Test " << test_no << " - Passed!" << std::endl;
    }
    else
    {
        std::cout << "❌ Test " << test_no << " - Failed!" << std::endl;
    }
}

void PrintNode(std::shared_ptr<tree::TreeNode<int>> node, unsigned int level)
{
    for(int i = 0; i < level; i++)
    {
        std::cout << "  ";
    }
    std::cout << "├─Node: " << node->Content() << "\n";
    for(auto i : node->Children())
    {
        PrintNode(i, level + 1);
    }
}

void PrintTree(tree::Tree<int>& theTree)
{
    for(auto i : theTree.RootNode())
    {
        PrintNode(i, 0);
    }
}

int main()
{
    unsigned int test_no = 1;
    int test_val = 42;
    // Create an empty tree
    tree::Tree<int> tree1;
    tree1.AddRootNode(test_val);

    Test(tree1.RootNode()[0].get()->Content() == test_val, test_no++);

    auto node1 = tree1.RootNode()[0];
    auto node2 = node1->Append(test_val);
    
    // Test operators (node vs. node)
    Test((*node1) == (*node2), test_no++);
    Test(!((*node1) != (*node2)), test_no++);
    Test(!((*node1) < (*node2)), test_no++);
    Test(!((*node1) > (*node2)), test_no++);
    Test((*node1) <= (*node2), test_no++);
    Test((*node1) >= (*node2), test_no++);

    // Test operators (node vs content);
    Test((*node1) == test_val, test_no++);
    Test(!((*node1) != test_val), test_no++);
    Test(!((*node1) < test_val), test_no++);
    Test(!((*node1) > test_val), test_no++);
    Test((*node1) <= test_val, test_no++);
    Test((*node1) >= test_val, test_no++);

    //Add nodes via append and loops
    for(int i = 0; i < 5; i++)
    {
        auto nodeLoop = node2->Append(i);
        for(int j = 0; j < 5; j++)
        {
            nodeLoop->Append(j);
        }
    }
    // Test two nodes for 5 children:
    Test(node2->Children().size() == 5, test_no++);
    Test(node2->Child(0)->Children().size() == 5, test_no++);
    // Print the tree
    PrintTree(tree1);
    Test(true, test_no++);

    // Drop a node
    node2->Child(2)->RecursiveDrop();
    PrintTree(tree1);
    Test(true, test_no++);

    // Create a new tree from part of the previous:
    tree::Tree<int> tree2(node2->Child(2)->Detatch());
    PrintTree(tree2);
    Test(true, test_no++);

    // Re-insert the tree node
    node2->Insert(tree2.RootNode()[0]->Detatch(), 1);
    PrintTree(tree1);
    Test(true, test_no++);
    
    // Test the tree size
    Test(tree1.TreeSize() == 27, test_no++);

    // Clear tree2:
    tree2.Clear();
    Test(tree2.TreeSize() == 0, test_no++);

    return 0;
}
