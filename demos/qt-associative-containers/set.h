#ifndef SET_H
#define SET_H

#include "rbtree.h"

template <typename Key, typename Compare = std::less<Key>,
          typename Allocator = std::allocator<Key>>
class Set : public RBTree<Set<Key, Compare, Allocator>, Key, Key, Compare> {
    using Base = RBTree<Set<Key, Compare, Allocator>, Key, Key, Compare>;
    using typename Base::Node;
    //friend Base;

   public:
    class iterator {
       protected:
        typename Base::iterator base_it;

       public:
        using iterator_category = std::bidirectional_iterator_tag;
        using value_type = Key;
        using difference_type = std::ptrdiff_t;
        using pointer = const Key*;
        using reference = const Key&;

        explicit iterator(
            typename Base::iterator it = typename Base::iterator())
            : base_it(it) {}

        // Non-const versions
        reference operator*() const { return base_it->first; }
        pointer operator->() const { return &base_it->first; }

        iterator& operator++() {
            ++base_it;
            return *this;
        }
        iterator operator++(int) {
            iterator tmp = *this;
            ++base_it;
            return tmp;
        }
        iterator& operator--() {
            --base_it;
            return *this;
        }
        iterator operator--(int) {
            iterator tmp = *this;
            --base_it;
            return tmp;
        }

        bool operator==(const iterator& other) const {
            return base_it == other.base_it;
        }
        bool operator!=(const iterator& other) const {
            return base_it != other.base_it;
        }

        typename Base::iterator get_base() const { return base_it; }
    };

    class const_iterator {
       protected:
        typename Base::const_iterator base_it;

       public:
        using iterator_category = std::bidirectional_iterator_tag;
        using value_type = const Key;
        using difference_type = std::ptrdiff_t;
        using pointer = const Key*;
        using reference = const Key&;

        explicit const_iterator(
            typename Base::const_iterator it = typename Base::const_iterator())
            : base_it(it) {}

        // Conversion from iterator to const_iterator
        const_iterator(const iterator& other) : base_it(other.get_base()) {}

        reference operator*() const { return base_it->first; }
        pointer operator->() const { return &base_it->first; }

        const_iterator& operator++() {
            ++base_it;
            return *this;
        }
        const_iterator operator++(int) {
            const_iterator tmp = *this;
            ++base_it;
            return tmp;
        }
        const_iterator& operator--() {
            --base_it;
            return *this;
        }
        const_iterator operator--(int) {
            const_iterator tmp = *this;
            --base_it;
            return tmp;
        }

        bool operator==(const const_iterator& other) const {
            return base_it == other.base_it;
        }
        bool operator!=(const const_iterator& other) const {
            return base_it != other.base_it;
        }
    };

    Set() = default;
    Set(const Set& other) : Base(other) {}
    Set(Set&& other) noexcept : Base(std::move(other)) {}
    ~Set() = default;

    Set& operator=(const Set& other) {
        Base::operator=(other);
        return *this;
    }
    Set& operator=(Set&& other) noexcept {
        Base::operator=(std::move(other));
        return *this;
    }

    std::pair<iterator, bool> insert(const Key& key) {
        auto result = this->insertImpl(key, key);
        return {iterator(result.first), result.second};
    }


    iterator erase(iterator pos) {
        return iterator(this->eraseImpl(pos.get_base()));
    }


    size_t erase(const Key& key) {
        auto it = find(key);
        if (it != end()) {
            erase(it);
            return 1;
        }
        return 0;
    }


    iterator find(const Key& key) { return iterator(this->findImpl(key)); }


    const_iterator find(const Key& key) const {
        return const_iterator(this->findImpl(key));
    }


    bool contains(const Key& key) const { return find(key) != end(); }

    iterator begin() noexcept { return iterator(Base::begin()); }

    const_iterator begin() const noexcept {
        return const_iterator(Base::begin());
    }

    iterator end() noexcept { return iterator(Base::end()); }

    const_iterator end() const noexcept { return const_iterator(Base::end()); }

    const_iterator cbegin() const noexcept { return begin(); }
    const_iterator cend() const noexcept { return end(); }
};

#endif	// SET_H
