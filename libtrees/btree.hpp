/**
 * \file    btree.hpp
 * \brief   Contains a general purpose binary tree structures.
 * 
 * \details This Binary Tree class can hold any type of data. It has special
 *          insert functions to maintain a 0 <= children <= 2 rule.
 *
 * \warning When assigning parents to nodes, this class does
 *          NOT check for circular dependencies. Infinitely
 *          looping node trees are possible. This may be a
 *          desired effect (i.e. a circle of events to
 *          continuely loop through). However, this is C++,
 *          it's up to you to be careful with this stuff.
 *          Just don't say I didn't warn you...
 *
 * \author Ethan Snyder
 * \date   2020
 */

#include <utility>
#include <algorithm>
#include <stdexcept>
#include <memory>

namespace tree
{
    template<typename T> class BTreeNode : public std::enable_shared_from_this<tree::BTreeNode<T>>
    {
        private:
            T m_content;
            std::weak_ptr<tree::BTreeNode<T>> m_parent;
            std::pair<std::shared_ptr<tree::BTreeNode<T>>,std::shared_ptr<tree::BTreeNode<T>>> m_children;
        public:
            // Constructors:

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
            BTreeNode(T& value);
            BTreeNode(T& value, std::shared_ptr<tree::BTreeNode<T>> parent);

            std::shared_ptr<tree::BTreeNode<T>> GetSharedPtr();

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
            std::shared_ptr<tree::BTreeNode<T>> Insert(T value, unsigned int index);

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
            std::shared_ptr<tree::BTreeNode<T>> Insert(std::shared_ptr<tree::BTreeNode<T>> node, unsigned int index);
            std::shared_ptr<tree::BTreeNode<T>> Append(T value);
            std::shared_ptr<tree::BTreeNode<T>> Append(std::shared_ptr<tree::BTreeNode<T>> node);

            /**
             * \brief  Removes a child by index
             *
             * \detail Removes a child at a given index. If the index given is
             *         out of range, a std::out_of_range exception will be
             *         thrown.
             *
             * \param index The index in the vector to be removed.
             *
             * \throw This function throws an std::out_of_range exception if the
             *        index is out of bounds.
             */
            void RemoveChild(unsigned int index);

            /**
             * \brief  Removes a child by finding it
             *
             * \detail Checks the children of a node for the given child. If the
             *         child does not exist, nothing happens.
             * 
             * \param node The node to searched for and removed.
             */
            void RemoveChild(std::shared_ptr<tree::BTreeNode<T>> node);

            /**
             * \brief Recursively delete all children from current node.
             *
             * \detail This deletes all the children nodes recursively, if any.
             */
            void ClearChildren();

            /**
             * \brief  Detaches the current node with children from the tree.
             *
             * \detail The current node gets detached from the tree with its children intact.
             */
            std::shared_ptr<tree::BTreeNode<T>> Detatch();

            int FindChild(std::shared_ptr<tree::BTreeNode<T>> node);

            // Deletes current node and returns the children of the dropped node
            std::pair<std::shared_ptr<tree::BTreeNode<T>>, std::shared_ptr<tree::BTreeNode<T>>> Drop();
            // Deletes the current node and all of it's children
            void RecursiveDrop();

        // Static functions
        public:
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
             * \param  NodeA Do I really have to describe what these two parameters are?
             *
             * \param  NodeB See above^^^
             */
            static void Swap(std::shared_ptr<tree::BTreeNode<T>> NodeA, std::shared_ptr<tree::BTreeNode<T>> NodeB);

        // Operators
        public:
            // Shortcut content assignment
            std::shared_ptr<tree::BTreeNode<T>> operator=(const T content);
            // Shortcut to append
            std::shared_ptr<tree::BTreeNode<T>> operator<<(const T content);
            std::shared_ptr<tree::BTreeNode<T>> operator<<(std::shared_ptr<tree::BTreeNode<T>> node);
            // Comparisons
            friend bool operator==(const tree::BTreeNode<T>& lhs, const tree::BTreeNode<T>& rhs);
            friend bool operator==(const tree::BTreeNode<T>& lhs, const T& rhs);
            friend bool operator!=(const tree::BTreeNode<T>& lhs, const tree::BTreeNode<T>& rhs);
            friend bool operator!=(const tree::BTreeNode<T>& lhs, const T& rhs);
            friend bool operator< (const tree::BTreeNode<T>& lhs, const tree::BTreeNode<T>& rhs);
            friend bool operator< (const tree::BTreeNode<T>& lhs, const T& rhs);
            friend bool operator> (const tree::BTreeNode<T>& lhs, const tree::BTreeNode<T>& rhs);
            friend bool operator> (const tree::BTreeNode<T>& lhs, const T& rhs);
            friend bool operator<=(const tree::BTreeNode<T>& lhs, const tree::BTreeNode<T>& rhs);
            friend bool operator<=(const tree::BTreeNode<T>& lhs, const T& rhs);
            friend bool operator>=(const tree::BTreeNode<T>& lhs, const tree::BTreeNode<T>& rhs);
            friend bool operator>=(const tree::BTreeNode<T>& lhs, const T& rhs);

    };

    template<typename T> class BTree
    {
        private:
            std::shared_ptr<tree::BTreeNode<T>> m_root;
        public:
            /** brief Tree - Creates an empty tree
             *  
             */
            BTree();
            BTree(std::shared_ptr<tree::BTreeNode<T>> node);
            BTree(const T content);
            ~BTree();
            
            void Clear();
            std::size_t TreeSize();
            std::size_t TreeSize(std::shared_ptr<tree::BTreeNode<T>> startingNode);

            std::shared_ptr<tree::BTreeNode<T>> RootNode() {return m_root;}
    };
}

/******************************************************************************/
/*                _____                _   _           _                      */
/*               |_   _| __ ___  ___  | \ | | ___   __| | ___                 */
/*                 | || '__/ _ \/ _ \ |  \| |/ _ \ / _` |/ _ \                */
/*                 | || | |  __/  __/ | |\  | (_) | (_| |  __/                */
/*                 |_||_|  \___|\___| |_| \_|\___/ \__,_|\___|                */
/*                                                                            */
/*                _____                 _   _                                 */
/*               |  ___|   _ _ __   ___| |_(_) ___  _ __  ___                 */
/*               | |_ | | | | '_ \ / __| __| |/ _ \| '_ \/ __|                */
/*               |  _|| |_| | | | | (__| |_| | (_) | | | \__ \                */
/*               |_|   \__,_|_| |_|\___|\__|_|\___/|_| |_|___/                */
/******************************************************************************/

// Constructors
template<typename T>
tree::BTreeNode<T>::BTreeNode(T& value)
{
    m_content = value;
}

template<typename T>
tree::BTreeNode<T>::BTreeNode(T& value, std::shared_ptr<tree::BTreeNode<T>> parent)
{
    m_content = value;
    m_parent = parent;
}

template<typename T>
std::shared_ptr<tree::BTreeNode<T>> tree::BTreeNode<T>::GetSharedPtr()
{
    return this->shared_from_this();
}

template<typename T>
std::shared_ptr<tree::BTreeNode<T>> tree::BTreeNode<T>::Insert(T value, unsigned int index)
{
    std::shared_ptr<tree::BTreeNode<T>> ret(nullptr);
    if(index <= m_children.size())
    {
        // Insert at requested location
        ret.reset(new tree::BTreeNode<T>(value, this));
        m_children.insert(m_children.begin()+index, ret);
    }
    else
    {
        // Append instead
        ret.reset(this->Append(value));
    }

    return ret;
}


template<typename T>
std::shared_ptr<tree::BTreeNode<T>> tree::BTreeNode<T>::Insert(std::shared_ptr<tree::BTreeNode<T>> node, unsigned int index)
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

    return node;
}

template<typename T>
std::shared_ptr<tree::BTreeNode<T>> tree::BTreeNode<T>::Append(T value)
{
    std::shared_ptr<tree::BTreeNode<T>> node(new tree::BTreeNode<T>(value, this));
    m_children.push_back(node);

    return node;
}

template<typename T>
std::shared_ptr<tree::BTreeNode<T>> tree::BTreeNode<T>::Append(std::shared_ptr<tree::BTreeNode<T>> node)
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

template<typename T>
std::shared_ptr<tree::BTreeNode<T>> tree::BTreeNode<T>::Detatch()
{
    if(m_parent != nullptr)
    {
        if(auto parent = m_parent.lock())
        {
            parent->RemoveChild(this->GetSharedPtr());
        }
        else
        {
            // Parent does not exist
            return nullptr;
        }
    }
    return this->GetSharedPtr();
}

template<typename T>
void tree::BTreeNode<T>::RemoveChild(unsigned int index)
{
    if(index >= m_children.size())
    {
        throw std::out_of_range("Index out of range");
    }
    else
    {
        m_children.erase(m_children.begin() + index);
    }
}

template<typename T>
void tree::BTreeNode<T>::RemoveChild(std::shared_ptr<tree::BTreeNode<T>> node)
{
    auto result = std::find(m_children.begin(), m_children.end(), node);
    if(result != m_children.end())
    {
        m_children.erase(result);
    }
}

template<typename T>
void tree::BTreeNode<T>::ClearChildren()
{
    m_children.clear();
}

template<typename T>
std::vector<std::shared_ptr<tree::BTreeNode<T>>> tree::BTreeNode<T>::Drop()
{
    if(auto parent = m_parent.lock())
    {
        parent->RemoveChild(this->GetSharedPtr());
    }
    return m_children;
}

template<typename T>
void tree::BTreeNode<T>::RecursiveDrop()
{
    for(auto child : m_children)
    {
        child->RecursiveDrop();
    }

    m_children.clear();
}

template<typename T>
int tree::BTreeNode<T>::FindChild(std::shared_ptr<tree::BTreeNode<T>> node)
{
    auto result = std::find(m_children.begin(), m_children.end(), node);
    return (result == m_children.end()) ? -1 : (m_children.end() - result);
}


template<typename T>
void tree::BTreeNode<T>::Swap(std::shared_ptr<tree::BTreeNode<T>> NodeA, std::shared_ptr<tree::BTreeNode<T>> NodeB)
{
    // This only swaps the contents(value) of the nodes
    T temp = NodeA->m_content;
    NodeA->m_content = NodeB->m_content;
    NodeB->m_content = temp;
}

// Operators

template<typename T>
std::shared_ptr<tree::BTreeNode<T>> tree::BTreeNode<T>::operator=(const T content)
{
    this->m_content = content;
    return this->GetSharedPtr();
}

template<typename T>
std::shared_ptr<tree::BTreeNode<T>> tree::BTreeNode<T>::operator<<(const T content)
{
    return this->Append(content);
}

template<typename T>
std::shared_ptr<tree::BTreeNode<T>> tree::BTreeNode<T>::operator<<(std::shared_ptr<tree::BTreeNode<T>> node)
{
    return this->Append(node);
}

template<typename T>
bool operator==(const tree::BTreeNode<T>& lhs, const tree::BTreeNode<T>& rhs)
{
    return lhs.m_content == rhs.m_content;
}

template<typename T>
bool operator==(const tree::BTreeNode<T>& lhs, const T& rhs)
{
    return lhs.m_content == rhs;
}

template<typename T>
bool operator!=(const tree::BTreeNode<T>& lhs, const tree::BTreeNode<T>& rhs)
{
    return !(lhs.m_content == rhs.m_content);
}

template<typename T>
bool operator!=(const tree::BTreeNode<T>& lhs, const T& rhs)
{
    return !(lhs.m_content == rhs);
}

template<typename T>
bool operator< (const tree::BTreeNode<T>& lhs, const tree::BTreeNode<T>& rhs)
{
    return (lhs.m_content < rhs.m_content);
}

template<typename T>
bool operator< (const tree::BTreeNode<T>& lhs, const T& rhs)
{
    return (lhs.m_content < rhs);
}

template<typename T>
bool operator> (const tree::BTreeNode<T>& lhs, const tree::BTreeNode<T>& rhs)
{
    return (lhs.m_content > rhs.m_content);
}

template<typename T>
bool operator> (const tree::BTreeNode<T>& lhs, const T& rhs)
{
    return (lhs.m_content > rhs);
}

template<typename T>
bool operator<=(const tree::BTreeNode<T>& lhs, const tree::BTreeNode<T>& rhs)
{
    return (lhs.m_content <= rhs.m_content);
}

template<typename T>
bool operator<=(const tree::BTreeNode<T>& lhs, const T& rhs)
{
    return (lhs.m_content <= rhs);
}

template<typename T>
bool operator>=(const tree::BTreeNode<T>& lhs, const tree::BTreeNode<T>& rhs)
{
    return (lhs.m_content >= rhs.m_content);
}

template<typename T>
bool operator>=(const tree::BTreeNode<T>& lhs, const T& rhs)
{
    return (lhs.m_content >= rhs);
}

//------------------------------------------------------------------------------
// Trees
//------------------------------------------------------------------------------

template<typename T>
tree::BTree<T>::BTree()
{
}

template<typename T>
tree::BTree<T>::BTree(std::shared_ptr<tree::BTreeNode<T>> node)
{
    m_root.push_back(node);
}

template<typename T>
tree::BTree<T>::BTree(const T content)
{
    m_root.push_back(std::shared_ptr<tree::BTreeNode<T>>(new tree::BTreeNode<T>(content)));
}

template<typename T>
void tree::BTree<T>::Clear()
{
    m_root.clear();
}

template<typename T>
std::size_t tree::BTree<T>::TreeSize()
{
    // Go through the entire tree
    return 0;
}

template<typename T>
std::size_t tree::BTree<T>::TreeSize(std::shared_ptr<tree::BTreeNode<T>> startingNode)
{
    return 0;
}

template<typename T>
void tree::BTree<T>::AddRootNode(std::shared_ptr<tree::BTreeNode<T>> node, int index)
{
    if(node.get() == nullptr)
    {
        throw  std::invalid_argument("Cannot pass nullptr");
    }
    
    if((index < 0) || (index > m_root.size()))
    {
        m_root.push_back(node);
    }
    else
    {
        m_root.insert(m_root.begin + index, node);
    }
}

template<typename T>
void tree::BTree<T>::AddRootNode(const T content, int index)
{
    if((index < 0) || (index > m_root.size()))
    {
        m_root.push_back(std::shared_ptr<tree::BTreeNode<T>>(new tree::BTreeNode<T>(content)));
    }
    else
    {
        m_root.insert(m_root.begin + index, std::shared_ptr<tree::BTreeNode<T>>(new tree::BTreeNode<T>(content)));
    }
}
