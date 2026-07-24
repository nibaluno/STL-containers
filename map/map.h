#ifndef MAP_H
#define MAP_H

#include <stdexcept>
#include <utility>
#include "rbtree.h"

template <typename Key, typename T, typename Compare = std::less<Key>,
          typename Allocator = std::allocator<std::pair<const Key, T>>>
class Map : public RBTree<Map<Key, T, Compare, Allocator>, Key, T, Compare> {
    using Base = RBTree<Map<Key, T, Compare, Allocator>, Key, T, Compare>;
    using typename Base::Node;


   public:
    using key_type = Key;
    using mapped_type = T;
    using value_type = std::pair<const Key, T>;
    using size_type = size_t;
    using key_compare = Compare;
    using allocator_type = Allocator;
    using reference = value_type&;
    using const_reference = const value_type&;
    using pointer = typename std::allocator_traits<Allocator>::pointer;
    using const_pointer =
        typename std::allocator_traits<Allocator>::const_pointer;

    using iterator = typename Base::iterator;
    using const_iterator = typename Base::const_iterator;

    Map() = default;
    Map(const Map& other) : Base(other) {}
    Map(Map&& other) noexcept : Base(std::move(other)) {}
    ~Map() = default;

    Map& operator=(const Map& other) {
        Base::operator=(other);
        return *this;
    }
    Map& operator=(Map&& other) noexcept {
        Base::operator=(std::move(other));
        return *this;
    }

    T& at(const Key& key) {
        auto it = this->findImpl(key);
        if (it == this->end())
            throw std::out_of_range("Key not found");
        return it->second;
    }

    const T& at(const Key& key) const {
        auto it = this->findImpl(key);
        if (it == this->end())
            throw std::out_of_range("Key not found");
        return it->second;
    }

    T& operator[](const Key& key) {
        auto result = this->insertImpl(key, T());
        return result.first->second;
    }

    std::pair<iterator, bool> insert(const value_type& value) {
        return this->insertImpl(value.first, value.second);
    }

    iterator erase(iterator pos) { return this->eraseImpl(pos); }

    size_type erase(const Key& key) {
        auto it = this->findImpl(key);
        if (it != this->end()) {
            this->eraseImpl(it);
            return 1;
        }
        return 0;
    }

    iterator find(const Key& key) { return this->findImpl(key); }

    const_iterator find(const Key& key) const { return this->findImpl(key); }

    bool contains(const Key& key) const {
        return this->findImpl(key) != this->end();
    }

    using Base::begin;
    using Base::clear;
    using Base::empty;
    using Base::end;
    using Base::size;
};

#endif	// MAP_H
