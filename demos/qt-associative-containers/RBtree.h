#ifndef RBTREE_H
#define RBTREE_H

#include <functional>
#include <utility>

template <typename Derived, typename Key, typename Value,
          typename Compare = std::less<Key>>
class RBTree {
   protected:
    struct Node {
        std::pair<const Key, Value> data;
        Node* left;
        Node* right;
        Node* parent;
        bool is_red;

        Node(const Key& key, const Value& value, Node* parent, bool is_red)
            : data(std::piecewise_construct, std::forward_as_tuple(key),
                   std::forward_as_tuple(value)),
              left(nullptr),
              right(nullptr),
              parent(parent),
              is_red(is_red) {}
    };

    Node* root_;
    size_t size_;
    Compare comp_;


    void rotateLeft(Node* node) {
        Node* child = node->right;
        node->right = child->left;

        if (child->left) {
            child->left->parent = node;
        }

        child->parent = node->parent;

        if (!node->parent) {
            root_ = child;
        } else if (node == node->parent->left) {
            node->parent->left = child;
        } else {
            node->parent->right = child;
        }

        child->left = node;
        node->parent = child;
    }


    void rotateRight(Node* node) {
        Node* child = node->left;
        node->left = child->right;

        if (child->right) {
            child->right->parent = node;
        }

        child->parent = node->parent;

        if (!node->parent) {
            root_ = child;
        } else if (node == node->parent->right) {
            node->parent->right = child;
        } else {
            node->parent->left = child;
        }

        child->right = node;
        node->parent = child;
    }


    void fixInsert(Node* node) {
        while (node != root_ && node->parent->is_red) {
            Node* parent = node->parent;
            Node* grandparent = parent->parent;

            if (parent == grandparent->left) {
                Node* uncle = grandparent->right;

                if (uncle && uncle->is_red) {
                    parent->is_red = false;
                    uncle->is_red = false;
                    grandparent->is_red = true;
                    node = grandparent;
                } else {
                    if (node == parent->right) {
                        node = parent;
                        rotateLeft(node);
                        parent = node->parent;
                    }

                    parent->is_red = false;
                    grandparent->is_red = true;
                    rotateRight(grandparent);
                }
            } else {
                Node* uncle = grandparent->left;

                if (uncle && uncle->is_red) {
                    parent->is_red = false;
                    uncle->is_red = false;
                    grandparent->is_red = true;
                    node = grandparent;
                } else {
                    if (node == parent->left) {
                        node = parent;
                        rotateRight(node);
                        parent = node->parent;
                    }

                    parent->is_red = false;
                    grandparent->is_red = true;
                    rotateLeft(grandparent);
                }
            }
        }

        if (root_) {
            root_->is_red = false;
        }
    }

    Node* minValueNode(Node* node) const {
        if (!node)
            return nullptr;


        while (node->left) {
            node = node->left;
        }
        return node;
    }

    Node* deleteNode(Node* node) {
        Node* replacement = nullptr;
        bool original_color = node->is_red;


        if (!node->left) {
            replacement = node->right;
            transplant(node, node->right);
        } else if (!node->right) {
            replacement = node->left;
            transplant(node, node->left);
        } else {
            Node* successor = minValueNode(node->right);
            original_color = successor->is_red;
            replacement = successor->right;

            if (successor->parent == node) {
                if (replacement) {
                    replacement->parent = successor;
                }
            } else {
                transplant(successor, successor->right);
                successor->right = node->right;
                successor->right->parent = successor;
            }

            transplant(node, successor);
            successor->left = node->left;
            successor->left->parent = successor;
            successor->is_red = node->is_red;
        }

        delete node;
        return replacement;
    }


    void transplant(Node* u, Node* v) {
        if (!u->parent) {
            root_ = v;
        } else if (u == u->parent->left) {
            u->parent->left = v;
        } else {
            u->parent->right = v;
        }

        if (v) {
            v->parent = u->parent;
        }
    }


    void fixDelete(Node* node) {
        while (node != root_ && (!node || !node->is_red)) {
            if (node == node->parent->left) {
                Node* sibling = node->parent->right;

                if (sibling->is_red) {
                    sibling->is_red = false;
                    node->parent->is_red = true;
                    rotateLeft(node->parent);
                    sibling = node->parent->right;
                }

                if ((!sibling->left || !sibling->left->is_red) &&
                    (!sibling->right || !sibling->right->is_red)) {
                    sibling->is_red = true;
                    node = node->parent;
                } else {
                    if (!sibling->right || !sibling->right->is_red) {
                        if (sibling->left)
                            sibling->left->is_red = false;
                        sibling->is_red = true;
                        rotateRight(sibling);
                        sibling = node->parent->right;
                    }

                    sibling->is_red = node->parent->is_red;
                    node->parent->is_red = false;
                    if (sibling->right)
                        sibling->right->is_red = false;
                    rotateLeft(node->parent);
                    node = root_;
                }
            } else {
                Node* sibling = node->parent->left;

                if (sibling->is_red) {
                    sibling->is_red = false;
                    node->parent->is_red = true;
                    rotateRight(node->parent);
                    sibling = node->parent->left;
                }

                if ((!sibling->right || !sibling->right->is_red) &&
                    (!sibling->left || !sibling->left->is_red)) {
                    sibling->is_red = true;
                    node = node->parent;
                } else {
                    if (!sibling->left || !sibling->left->is_red) {
                        if (sibling->right)
                            sibling->right->is_red = false;
                        sibling->is_red = true;
                        rotateLeft(sibling);
                        sibling = node->parent->left;
                    }

                    sibling->is_red = node->parent->is_red;
                    node->parent->is_red = false;
                    if (sibling->left)
                        sibling->left->is_red = false;
                    rotateRight(node->parent);
                    node = root_;
                }
            }
        }

        if (node) {
            node->is_red = false;
        }
    }


    void clearRecursive(Node* node) {
        if (!node)
            return;
        clearRecursive(node->left);
        clearRecursive(node->right);
        delete node;
    }

   public:
    template <typename T>
    class iterator_base {
       protected:
        Node* current;

       public:
        using iterator_category = std::bidirectional_iterator_tag;
        using value_type = T;
        using difference_type = std::ptrdiff_t;
        using pointer = T*;
        using reference = T&;

        explicit iterator_base(Node* node = nullptr) : current(node) {}

        template <typename U,
                  typename = std::enable_if_t<std::is_same_v<T, const U>>>
        iterator_base(const iterator_base<U>& other) : current(other.current) {}

        reference operator*() { return current->data; }
        pointer operator->() { return &current->data; }

        const value_type& operator*() const { return current->data; }
        const value_type* operator->() const { return &current->data; }

        iterator_base& operator++() {
            if (!current)
                return *this;

            if (current->right) {
                current = current->right;
                while (current->left) {
                    current = current->left;
                }
            } else {
                Node* parent = current->parent;
                while (parent && current == parent->right) {
                    current = parent;
                    parent = parent->parent;
                }
                current = parent;
            }
            return *this;
        }

        iterator_base operator++(int) {
            iterator_base tmp = *this;
            ++(*this);
            return tmp;
        }

        iterator_base& operator--() {
            if (!current)
                return *this;

            if (current->left) {
                current = current->left;
                while (current->right) {
                    current = current->right;
                }
            } else {
                Node* parent = current->parent;
                while (parent && current == parent->left) {
                    current = parent;
                    parent = parent->parent;
                }
                current = parent;
            }
            return *this;
        }

        iterator_base operator--(int) {
            iterator_base tmp = *this;
            --(*this);
            return tmp;
        }

        bool operator==(const iterator_base& other) const {
            return current == other.current;
        }

        bool operator!=(const iterator_base& other) const {
            return current != other.current;
        }

        Node* get_node() const { return current; }
    };

    using iterator = iterator_base<std::pair<const Key, Value>>;
    using const_iterator = iterator_base<const std::pair<const Key, Value>>;

    RBTree() : root_(nullptr), size_(0) {}
    ~RBTree() { clear(); }

    iterator begin() noexcept {
        Node* current = root_;


        if (current) {
            while (current->left) {
                current = current->left;
            }
        }
        return iterator(current);
    }

    const_iterator begin() const noexcept {
        Node* current = root_;


        if (current) {
            while (current->left) {
                current = current->left;
            }
        }
        return const_iterator(current);
    }

    iterator end() noexcept { return iterator(nullptr); }
    const_iterator end() const noexcept { return const_iterator(nullptr); }


    size_t size() const { return size_; }


    bool empty() const { return size_ == 0; }


    void clear() {
        clearRecursive(root_);
        root_ = nullptr;
        size_ = 0;
    }

   protected:
    std::pair<iterator, bool> insertImpl(const Key& key, const Value& value) {
        Node* parent = nullptr;
        Node* current = root_;


        while (current) {
            parent = current;
            if (comp_(key, current->data.first)) {
                current = current->left;
            } else if (comp_(current->data.first, key)) {
                current = current->right;
            } else {
                return {iterator(current), false};
            }
        }

        Node* new_node = new Node(key, value, parent, true);


        if (!parent) {
            root_ = new_node;
        } else if (comp_(key, parent->data.first)) {
            parent->left = new_node;
        } else {
            parent->right = new_node;
        }

        fixInsert(new_node);
        size_++;

        return {iterator(new_node), true};
    }


    iterator eraseImpl(iterator pos) {
        if (pos == end())
            return end();

        Node* node = pos.get_node();
        Node* next_node = node;

        if (node->right) {
            next_node = minValueNode(node->right);
        } else {
            next_node = node->parent;
        }

        Node* replacement = deleteNode(node);

        if (replacement && !replacement->is_red) {
            fixDelete(replacement);
        } else if (!replacement && node->parent) {
            fixDelete(node->parent);
        }

        size_--;
        return iterator(next_node);
    }


    iterator findImpl(const Key& key) {
        Node* current = root_;
        while (current) {
            if (comp_(key, current->data.first)) {
                current = current->left;
            } else if (comp_(current->data.first, key)) {
                current = current->right;
            } else {
                return iterator(current);
            }
        }
        return end();
    }


    const_iterator findImpl(const Key& key) const {
        Node* current = root_;
        while (current) {
            if (comp_(key, current->data.first)) {
                current = current->left;
            } else if (comp_(current->data.first, key)) {
                current = current->right;
            } else {
                return const_iterator(current);
            }
        }
        return end();
    }
};

#endif	// RBTREE_H
