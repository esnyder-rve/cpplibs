/**
 * \file tree.hpp
 * \brief Contains a general purpose tree structure.
 * 
 * \details Trees can hold any type of data, but only of one
 *          type. The tree nodes can have as many or as few
 *          nodes as you like. For Binary Trees, see BTree.hpp.
 *
 * \warning When assigning parents to nodes, this class does
 *          NOT check for circular dependencies. Infinitely
 *          looping node trees are possible. This may be a
 *          desired effect (i.e. a circle of events to
 *          continuely loop through). However, this is C++,
 *          it's up to you to be careful with this stuff.
 *          Just don't say I didn't warn you...
 *
 * \remark  This class was originally created for holding an
 *          HTML document as a DOM.
 *
 * \author Ethan Snyder
 * \date   2020
 */

#include <vector>
#include <memory>
#include <stdexcept>

namespace tree
{
    template<typename T> class TreeNode : public std::enable_shared_from_this<tree::TreeNode<T>>
    {
        private:
            T m_content;
            std::weak_ptr<tree::TreeNode<T>> m_parent;
            //tree::TreeNode<T> *m_parent;
            std::vector<std::shared_ptr<tree::TreeNode<T>>> m_children;
        public:
            // Constructors:
            TreeNode(T& value);
            TreeNode(T& value, tree::TreeNode<T>* parent);

            std::shared_ptr<tree::TreeNode<T>> GetPtr();

            tree::TreeNode<T>* Insert(T value, int index);
            tree::TreeNode<T>* Insert(tree::TreeNode<T>* node, int index);
            tree::TreeNode<T>* Append(T value);
            tree::TreeNode<T>* Append(tree::TreeNode<T>* node);
            void RemoveChild(int index);
            void RemoveChild(tree::TreeNode<T>* node);
            void ClearChildren();

            tree::TreeNode<T>* Detatch();

            // Deletes current node and adds children to the current node
            void Drop();
            // Deletes the current node and all of it's children
            void RecursiveDrop();

        // Static functions
        public:
            static void Swap(tree::TreeNode<T>& NodeA, tree::TreeNode<T>& NodeB);

    };

    template<typename T> class Tree
    {
        private:
            unsigned int m_nodeCount;
            tree::TreeNode<T> *m_rootNode;
        public:
            /** brief Tree - Creates an empty tree
             *  
             */
            Tree();
            ~Tree();
            
            void Clear();
            unsigned int Size();
    };
}

/**
 * \brief    create a stand-alone node with content
 *
 * \detail   create a node with an initial content.
 *
 * \tparam   t  any basic type or complex data structure
 *              or class.
 *
 * \param    value  what the node will contain.
 */
template<typename T>
tree::TreeNode<T>::TreeNode(T& value)
{
    m_content = value;
}

template<typename T>
tree::TreeNode<T>::TreeNode(T& value, tree::TreeNode<T>* parent)
{
    m_content = value;
    m_parent = parent;
}

/**
 * \brief Get a smart pointer refering to this very node.
 *
 * \detail
 */
template<typename T>
std::shared_ptr<tree::TreeNode<T>> tree::TreeNode<T>::GetPtr()
{
    return this->shared_from_this();
}

/**
 * \brief    Insert a new child node at specific index.
 *
 * \detail   Insert a new child node at a specific index.
 *           If the index given is greater that the number
 *           of children in the current node, the new child
 *           will be appended as the last child.
 *
 * \param[in]       value       The new child's content to be added.
 * \param[in]       index       The index in which to insert the child.
 *
 * \throw   This function throws an std::out_of_range exception if the
 *          given index is less than 0.
 *
 * \return  This function returns as a convience, a pointer
 *          to the new node.
 */

template<typename T>
tree::TreeNode<T>* tree::TreeNode<T>::Insert(T value, int index)
{
    tree::TreeNode<T>* ret = nullptr;
    if(index < 0)
    {
        if(index <= m_children.size())
        {
            // Insert at requested location
            auto node = new tree::TreeNode<T>(value, this);
            m_children.insert(m_children.begin()+index, &node);
            ret = &node;
        }
        else
        {
            // Append instead
            ret = this->Append(value);
        }
    }
    else
    {
        throw std::out_of_range("Index must be greater than 0");
    }

    return ret;
}

/**
 * \brief    Insert an existing node at specific index.
 *
 * \detail   Inserts an existing node at a specific index.
 *           If the index given is greater that the number
 *           of children in the current node, the child
 *           will be appended as the last child.
 *
 * \tparam   T      Any basic type or complex data structure
 *                  or class.
 * \param    node   The address of the node to insert.
 * \param    index  The index in which to insert the child.
 *
 * \throw   This function throws an std::out_of_range exception
 *          if the given index is less than 0.
 * \throw   This function throws an std::invalid_argument
 *          exception if the node is nullptr.
 *
 * \return  This function returns as a convience, a pointer
 *          to the newly inserted node.
 */

template<typename T>
tree::TreeNode<T>* tree::TreeNode<T>::Insert(tree::TreeNode<T>* node, int index)
{
    if(index > 0)
    {
        if(node != nullptr)
        {
            if(index <= m_children.size())
            {
                node->m_parent = this;
                m_children.insert(m_children.begin() + index, node);
            }
            else
            {
                // Instead of just erroring out if the index is out of range,
                // just append to the end
                this->Append(node);
            }
        }
        else
        {
            throw std::invalid_argument("Cannot pass nullptr");
        }
    }
    else
    {
        throw std::out_of_range("Index must be greater than 0");
    }

    return node;
}

template<typename T>
tree::TreeNode<T>* tree::TreeNode<T>::Append(T value)
{
   auto node = new tree::TreeNode<T>(value, this);
   m_children.push_back(&node);

   return &node;
}

template<typename T>
tree::TreeNode<T>* tree::TreeNode<T>::Append(tree::TreeNode<T>* node)
{
    if(node != nullptr)
    {
        node->m_parent = this;
        m_children.push_back(node);
    }
    else
    {
        // Throw exception.
        throw std::invalid_argument("Cannot pass nullptr");
    }

    return node;
}

/**
 * \brief  Detaches the current node with children from the tree.
 *
 * \detail The current node gets detached from the tree with its children intact.
 *         
 */
template<typename T>
tree::TreeNode<T>* tree::TreeNode<T>::Detatch()
{
    if(m_parent != nullptr)
    {
        m_parent->RemoveChild(this);
    }
    return this;
}

/**
 * \brief Swap two node contents
 *
 * \detail This function only swaps the contents of two nodes, it does not
 *         touch any children or parents. To move subtrees around, use Detach()
 *         and Append()/Insert() as they have very low overhead, with only moving
 *         pointers around and not possibly large data structures/classes.
 *
 * \tparam T Any basic type or complex data structure or class.
 *
 * \param  NodeA Do I really have to describe what these two parameters are for?
 *
 * \param  NodeB See above^^^
 */
template<typename T>
void tree::TreeNode<T>::Swap(tree::TreeNode<T>& NodeA, tree::TreeNode<T>& NodeB)
{
    // This only swaps the contents(value) of the nodes
    T temp = NodeA.m_content;
    NodeA.m_content = NodeB.m_content;
    NodeB.m_content = temp;
}
