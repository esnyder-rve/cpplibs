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
#include <algorithm>
#include <stdexcept>
#include <memory>

namespace tree
{
    template<typename T> class TreeNode : public std::enable_shared_from_this<tree::TreeNode<T>>
    {
        private:
            T m_content;
            std::weak_ptr<tree::TreeNode<T>> m_parent;
            std::vector<std::shared_ptr<tree::TreeNode<T>>> m_children;
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
            TreeNode(T& value);
            TreeNode(T& value, std::shared_ptr<tree::TreeNode<T>> parent);

            std::shared_ptr<tree::TreeNode<T>> GetSharedPtr();

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
            std::shared_ptr<tree::TreeNode<T>> Insert(T value, unsigned int index);

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
            std::shared_ptr<tree::TreeNode<T>> Insert(std::shared_ptr<tree::TreeNode<T>> node, unsigned int index);
            std::shared_ptr<tree::TreeNode<T>> Append(T value);
            std::shared_ptr<tree::TreeNode<T>> Append(std::shared_ptr<tree::TreeNode<T>> node);

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
            void RemoveChild(std::shared_ptr<tree::TreeNode<T>> node);

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
            std::shared_ptr<tree::TreeNode<T>> Detatch();

            int FindChild(std::shared_ptr<tree::TreeNode<T>> node);

            // Deletes current node and returns the children of the dropped node
            std::vector<std::shared_ptr<tree::TreeNode<T>>> Drop();
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
            static void Swap(tree::TreeNode<T>& NodeA, tree::TreeNode<T>& NodeB);

        // Operators
        public:
            // Shortcut content assignment
            tree::TreeNode<T>& operator=(const T content);
            // Shortcut to append
            tree::TreeNode<T>& operator<<(const T content);
            tree::TreeNode<T>& operator<<(std::shared_ptr<tree::TreeNode<T>> node);
            // Comparisons
            friend bool operator==(const tree::TreeNode<T>& lhs, const tree::TreeNode<T>& rhs);
            friend bool operator==(const tree::TreeNode<T>& lhs, const T& rhs);
            friend bool operator!=(const tree::TreeNode<T>& lhs, const tree::TreeNode<T>& rhs);
            friend bool operator!=(const tree::TreeNode<T>& lhs, const T& rhs);
            friend bool operator< (const tree::TreeNode<T>& lhs, const tree::TreeNode<T>& rhs);
            friend bool operator< (const tree::TreeNode<T>& lhs, const T& rhs);
            friend bool operator> (const tree::TreeNode<T>& lhs, const tree::TreeNode<T>& rhs);
            friend bool operator> (const tree::TreeNode<T>& lhs, const T& rhs);
            friend bool operator<=(const tree::TreeNode<T>& lhs, const tree::TreeNode<T>& rhs);
            friend bool operator<=(const tree::TreeNode<T>& lhs, const T& rhs);
            friend bool operator>=(const tree::TreeNode<T>& lhs, const tree::TreeNode<T>& rhs);
            friend bool operator>=(const tree::TreeNode<T>& lhs, const T& rhs);

    };

    template<typename T> class Tree
    {
        private:
            std::vector<std::shared_ptr<tree::TreeNode<T>>> m_root;
        public:
            /** brief Tree - Creates an empty tree
             *  
             */
            Tree();
            Tree(std::shared_ptr<tree::TreeNode<T>> node);
            ~Tree();
            
            void Clear();
            unsigned int TreeSize();
            unsigned int TreeSize(tree::TreeNode<T> *startingNode);

            void RootNode(tree::TreeNode<T> *node);
            std::vector<std::shared_ptr<tree::TreeNode<T>>> RootNode() {return m_root;}
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
tree::TreeNode<T>::TreeNode(T& value)
{
    m_content = value;
}

template<typename T>
tree::TreeNode<T>::TreeNode(T& value, std::shared_ptr<tree::TreeNode<T>> parent)
{
    m_content = value;
    m_parent = parent;
}

template<typename T>
std::shared_ptr<tree::TreeNode<T>> tree::TreeNode<T>::GetSharedPtr()
{
    return this->shared_from_this();
}

template<typename T>
std::shared_ptr<tree::TreeNode<T>> tree::TreeNode<T>::Insert(T value, unsigned int index)
{
    std::shared_ptr<tree::TreeNode<T>> ret(nullptr);
    if(index <= m_children.size())
    {
        // Insert at requested location
        ret.reset(new tree::TreeNode<T>(value, this));
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
std::shared_ptr<tree::TreeNode<T>> tree::TreeNode<T>::Insert(std::shared_ptr<tree::TreeNode<T>> node, unsigned int index)
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
std::shared_ptr<tree::TreeNode<T>> tree::TreeNode<T>::Append(T value)
{
    std::shared_ptr<tree::TreeNode<T>> node(new tree::TreeNode<T>(value, this));
    m_children.push_back(node);

    return node;
}

template<typename T>
std::shared_ptr<tree::TreeNode<T>> tree::TreeNode<T>::Append(std::shared_ptr<tree::TreeNode<T>> node)
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
std::shared_ptr<tree::TreeNode<T>> tree::TreeNode<T>::Detatch()
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
void tree::TreeNode<T>::RemoveChild(unsigned int index)
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
void tree::TreeNode<T>::RemoveChild(std::shared_ptr<tree::TreeNode<T>> node)
{
    auto result = std::find(m_children.begin(), m_children.end(), node);
    if(result != m_children.end())
    {
        m_children.erase(result);
    }
}

template<typename T>
void tree::TreeNode<T>::ClearChildren()
{
    m_children.clear();
}

template<typename T>
std::vector<std::shared_ptr<tree::TreeNode<T>>> tree::TreeNode<T>::Drop()
{
    if(auto parent = m_parent.lock())
    {
        parent->RemoveChild(this->GetSharedPtr());
    }
    return m_children;
}

template<typename T>
void tree::TreeNode<T>::RecursiveDrop()
{
    for(auto child : m_children)
    {
        child->RecursiveDrop();
    }

    m_children.clear();
}

template<typename T>
int tree::TreeNode<T>::FindChild(std::shared_ptr<tree::TreeNode<T>> node)
{
    auto result = std::find(m_children.begin(), m_children.end(), node);
    return (result == m_children.end()) ? -1 : (m_children.end() - result);
}


template<typename T>
void tree::TreeNode<T>::Swap(tree::TreeNode<T>& NodeA, tree::TreeNode<T>& NodeB)
{
    // This only swaps the contents(value) of the nodes
    T temp = NodeA.m_content;
    NodeA.m_content = NodeB.m_content;
    NodeB.m_content = temp;
}

// Operators

template<typename T>
tree::TreeNode<T>& tree::TreeNode<T>::operator=(const T content)
{
    this->m_content = content;
    return *this;
}

template<typename T>
bool operator==(const tree::TreeNode<T>& lhs, const tree::TreeNode<T>& rhs)
{
    return lhs.m_content == rhs.m_content;
}

template<typename T>
bool operator==(const tree::TreeNode<T>& lhs, const T& rhs)
{
    return lhs.m_content == rhs;
}

template<typename T>
bool operator!=(const tree::TreeNode<T>& lhs, const tree::TreeNode<T>& rhs)
{
    return !(lhs.m_content == rhs.m_content);
}

template<typename T>
bool operator!=(const tree::TreeNode<T>& lhs, const T& rhs)
{
    return !(lhs.m_content == rhs);
}

template<typename T>
bool operator< (const tree::TreeNode<T>& lhs, const tree::TreeNode<T>& rhs)
{
}

template<typename T>
bool operator< (const tree::TreeNode<T>& lhs, const T& rhs)
{
}

template<typename T>
bool operator> (const tree::TreeNode<T>& lhs, const tree::TreeNode<T>& rhs)
{
}

template<typename T>
bool operator> (const tree::TreeNode<T>& lhs, const T& rhs)
{
}

template<typename T>
bool operator<=(const tree::TreeNode<T>& lhs, const tree::TreeNode<T>& rhs)
{
}

template<typename T>
bool operator<=(const tree::TreeNode<T>& lhs, const T& rhs)
{
}

template<typename T>
bool operator>=(const tree::TreeNode<T>& lhs, const tree::TreeNode<T>& rhs)
{
}

template<typename T>
bool operator>=(const tree::TreeNode<T>& lhs, const T& rhs)
{
}

//------------------------------------------------------------------------------
// Trees
//------------------------------------------------------------------------------
