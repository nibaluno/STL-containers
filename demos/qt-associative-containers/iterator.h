#pragma once

#include <iterator> // For iterator tags

template<typename Tree, typename Node, typename ValueType>
class Iterator {
private:
    Node* node_; // Current node pointer

public:
    // Iterator traits - required for STL compatibility
    using iterator_category = std::bidirectional_iterator_tag;
    /*
     * Bidirectional iterator means:
     * - Can move forward (++) and backward (--)
     * - Can't do random access (like +n)
     * - Used by lists, sets, maps in STL
     */
    
    using value_type = ValueType; // Type of values we're iterating over
    using difference_type = std::ptrdiff_t; 
    /*
     * Signed integer type for:
     * - Distance between iterators
     * - Pointer arithmetic
     * - Required for STL algorithms
     */
    
    using pointer = ValueType*;   // Pointer to value type
    using reference = ValueType&; // Reference to value type

    // Constructors
    Iterator() : node_(nullptr) {} // End iterator
    explicit Iterator(Node* node) : node_(node) {} // Points to specific node

    // Dereference operators
    reference operator*() const { 
        return node_->data; // Returns reference to node's data
    }
    
    pointer operator->() const { 
        return &node_->data; // Returns pointer to node's data
    }

    // Prefix increment
    Iterator& operator++() {
        increment();
        return *this;
    }
    
    // Postfix increment
    Iterator operator++(int) {
        Iterator tmp = *this;
        increment();
        return tmp;
    }

    // Prefix decrement
    Iterator& operator--() {
        decrement();
        return *this;
    }
    
    // Postfix decrement
    Iterator operator--(int) {
        Iterator tmp = *this;
        decrement();
        return tmp;
    }

    // Comparison operators
    bool operator==(const Iterator& other) const {
        return node_ == other.node_;
    }
    
    bool operator!=(const Iterator& other) const {
        return node_ != other.node_;
    }

private:
    void increment() {
        if (!node_) return;
        
        // If right child exists, go to minimum node in right subtree
        if (node_->right) {
            node_ = node_->right.get();
            while (node_->left) {
                node_ = node_->left.get();
            }
        } 
        // Otherwise go up to first ancestor where we're in left subtree
        else {
            Node* parent = node_->parent;
            while (parent && node_ == parent->right.get()) {
                node_ = parent;
                parent = parent->parent;
            }
            node_ = parent;
        }
    }

    void decrement() {
        if (!node_) return;
        
        // If left child exists, go to maximum node in left subtree
        if (node_->left) {
            node_ = node_->left.get();
            while (node_->right) {
                node_ = node_->right.get();
            }
        } 
        // Otherwise go up to first ancestor where we're in right subtree
        else {
            Node* parent = node_->parent;
            while (parent && node_ == parent->left.get()) {
                node_ = parent;
                parent = parent->parent;
            }
            node_ = parent;
        }
    }
};

/* Example usage with BST:
 * 
 *        [5]
 *       /   \
 *     [3]   [8]
 *     / \    /
 *   [1][4] [6]
 * 
 * ++ for 5 → 6 (min in right subtree)
 * ++ for 4 → 5 (first right parent)
 * -- for 5 → 4 (max in left subtree)
 * -- for 6 → 5 (first left parent)
 */