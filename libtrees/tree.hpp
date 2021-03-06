/**
 * \file tree.hpp
 * \brief Contains a general purpose tree structure.
 * 
 * \details Trees can hold any type of data, but only of one
 *          type. The tree nodes can have as many or as few
 *          nodes as you like. For Binary Trees, see btree.hpp.
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
            TreeNode(const T& value)
            {
                m_content = value;
            }

            TreeNode(const T& value, std::shared_ptr<tree::TreeNode<T>> parent)
            {
                m_content = value;
                m_parent = parent;
            }

            T Content()
            {
                return m_content;
            }

            void Content(const T& content)
            {
                m_content = content;
            }

            std::shared_ptr<tree::TreeNode<T>> GetSharedPtr()
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
            std::shared_ptr<tree::TreeNode<T>> Insert(const T& value, unsigned int index)
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
            std::shared_ptr<tree::TreeNode<T>> Insert(const std::shared_ptr<tree::TreeNode<T>>& node, unsigned int index)
            {
                if(node != nullptr)
                {
                    if(index <= m_children.size())
                    {
                        node->m_parent = this->GetSharedPtr();
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

            std::shared_ptr<tree::TreeNode<T>> Append(const T& value)
            {
                std::shared_ptr<tree::TreeNode<T>> node(new tree::TreeNode<T>(value, this->GetSharedPtr()));
                m_children.push_back(node);

                return node;
            }

            std::shared_ptr<tree::TreeNode<T>> Append(const std::shared_ptr<tree::TreeNode<T>>& node)
            {
                if(node != nullptr)
                {
                    node->m_parent = this->GetSharedPtr();
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
            void RemoveChild(unsigned int index)
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

            /**
             * \brief  Removes a child by finding it
             *
             * \detail Checks the children of a node for the given child. If the
             *         child does not exist, nothing happens.
             * 
             * \param node The node to searched for and removed.
             */
            void RemoveChild(const std::shared_ptr<tree::TreeNode<T>>& node)
            {
                auto result = std::find(m_children.begin(), m_children.end(), node);
                if(result != m_children.end())
                {
                    m_children.erase(result);
                }
            }

            /**
             * \brief Recursively delete all children from current node.
             *
             * \detail This deletes all the children nodes recursively, if any.
             */
            void ClearChildren()
            {
                m_children.clear();
            }

            std::vector<std::shared_ptr<tree::TreeNode<T>>> Children()
            {
                return m_children;
            }

            std::shared_ptr<tree::TreeNode<T>> Child(int index)
            {
                if(m_children.size() == 0)
                {
                    // The node has no children, return a nullptr
                    return nullptr;
                }
                else if((index < 0) || ((unsigned int)index > m_children.size()))
                {
                    return m_children[m_children.size() - 1];
                }
                return m_children[index];
            }

            /**
             * \brief  Detaches the current node with children from the tree.
             *
             * \detail The current node gets detached from the tree with its children intact.
             */
            std::shared_ptr<tree::TreeNode<T>> Detatch()
            {
                if(m_parent.lock() != nullptr)
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

            int FindChild(const std::shared_ptr<tree::TreeNode<T>>& node)
            {
                auto result = std::find(m_children.begin(), m_children.end(), node);
                return (result == m_children.end()) ? -1 : (m_children.end() - result);
            }

            // Deletes current node and returns the children of the dropped node
            std::vector<std::shared_ptr<tree::TreeNode<T>>> Drop()
            {
                if(auto parent = m_parent.lock())
                {
                    parent->RemoveChild(this->GetSharedPtr());
                }
                return m_children;
            }
            // Deletes the current node and all of it's children
            void RecursiveDrop()
            {
                for(auto child : m_children)
                {
                    child->RecursiveDrop();
                }

                m_children.clear();
            }

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
            static void Swap(std::shared_ptr<tree::TreeNode<T>> NodeA, std::shared_ptr<tree::TreeNode<T>> NodeB)
            {
                // This only swaps the contents(value) of the nodes
                T temp = NodeA->m_content;
                NodeA->m_content = NodeB->m_content;
                NodeB->m_content = temp;
            }

        // Operators
        public:
            // Shortcut content assignment
            std::shared_ptr<tree::TreeNode<T>> operator=(const T content)
            {
                this->m_content = content;
                return this->GetSharedPtr();
            }

            // Comparisons
            template<typename U>
            friend bool operator==(const tree::TreeNode<U>& lhs, const tree::TreeNode<U>& rhs) {return lhs.m_content == rhs.m_content;}

            template<typename U>
            friend bool operator==(const tree::TreeNode<U>& lhs, const U& rhs) {return lhs.m_content == rhs;}

            template<typename U>
            friend bool operator!=(const tree::TreeNode<U>& lhs, const tree::TreeNode<U>& rhs) {return !(lhs == rhs);}

            template<typename U>
            friend bool operator!=(const tree::TreeNode<U>& lhs, const U& rhs) {return !(lhs == rhs);}

            template<typename U>
            friend bool operator< (const tree::TreeNode<U>& lhs, const tree::TreeNode<U>& rhs) {return lhs.m_content < rhs.m_content;}

            template<typename U>
            friend bool operator< (const tree::TreeNode<U>& lhs, const U& rhs) {return lhs.m_content < rhs;}

            template<typename U>
            friend bool operator> (const tree::TreeNode<U>& lhs, const tree::TreeNode<U>& rhs) {return lhs.m_content > rhs.m_content;}

            template<typename U>
            friend bool operator> (const tree::TreeNode<U>& lhs, const U& rhs) {return lhs.m_content < rhs;}

            template<typename U>
            friend bool operator<=(const tree::TreeNode<U>& lhs, const tree::TreeNode<U>& rhs) {return !(lhs > rhs);}

            template<typename U>
            friend bool operator<=(const tree::TreeNode<U>& lhs, const U& rhs) {return !(lhs > rhs);}

            template<typename U>
            friend bool operator>=(const tree::TreeNode<U>& lhs, const tree::TreeNode<U>& rhs) {return !(lhs < rhs);}

            template<typename U>
            friend bool operator>=(const tree::TreeNode<U>& lhs, const U& rhs) {return !(lhs < rhs);}
    };

    template<typename T> class Tree
    {
        private:
            std::vector<std::shared_ptr<tree::TreeNode<T>>> m_root;
        public:
            /** brief Tree - Creates an empty tree
             *  
             */
            Tree()
            {
            }

            Tree(const std::shared_ptr<tree::TreeNode<T>>& node)
            {
                m_root.push_back(node);
            }

            Tree(T content)
            {
                m_root.push_back(std::shared_ptr<tree::TreeNode<T>>(new tree::TreeNode<T>(content)));
            }
            
            void Clear()
            {
                m_root.clear();
            }
            
            std::size_t TreeSize(bool isCircular=false)
            {
                std::size_t nodeCount = 0;
                if(isCircular)
                {
                    // Not implemented yet
                    return nodeCount;
                }
                else
                {
                    for(auto i : m_root)
                    {
                        nodeCount += this->TreeSize(i, false);
                        nodeCount++;
                    }
                }
                // Go through the entire tree
                return nodeCount;
            }

            std::size_t TreeSize(const std::shared_ptr<tree::TreeNode<T>>& startingNode, bool isCircular=false)
            {
                std::size_t nodeCount = 0;
                if(isCircular)
                {
                    return nodeCount;
                }
                else
                {
                    for(auto i : startingNode->Children())
                    {
                        nodeCount += TreeSize(i, false);
                        nodeCount++;
                    }
                }
                return nodeCount;
            }

            void AddRootNode(const std::shared_ptr<tree::TreeNode<T>>& node, int index = -1)
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

            void AddRootNode(const T& content, int index = -1)
            {
                if((index < 0) || (((unsigned int)index) > m_root.size()))
                {
                    m_root.push_back(std::shared_ptr<tree::TreeNode<T>>(new tree::TreeNode<T>(content)));
                }
                else
                {
                    m_root.insert(m_root.begin() + index, std::shared_ptr<tree::TreeNode<T>>(new tree::TreeNode<T>(content)));
                }
            }
            
            std::vector<std::shared_ptr<tree::TreeNode<T>>> RootNode()
            {
                return m_root;
            }
    };
}
