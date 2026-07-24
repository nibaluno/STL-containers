#ifndef UNORDERED_MAP_H
#define UNORDERED_MAP_H

#include <cmath>
#include <functional>
#include <list>
#include <memory>
#include <stdexcept>
#include <vector>
#include "unmapiterator.h"

template <typename Key, typename T, typename Hash = std::hash<Key>,
          typename KeyEqual = std::equal_to<Key>,
          typename Allocator = std::allocator<std::pair<const Key, T>>>
class unordered_map {
   public:
    using key_type = Key;
    using mapped_type = T;
    using value_type = std::pair<const Key, T>;
    using size_type = size_t;
    using difference_type = ptrdiff_t;
    using hasher = Hash;
    using key_equal = KeyEqual;
    using allocator_type = Allocator;
    using reference = value_type&;
    using const_reference = const value_type&;
    using pointer = typename std::allocator_traits<Allocator>::pointer;
    using const_pointer =
        typename std::allocator_traits<Allocator>::const_pointer;
    using iterator = unordered_map_iterator<Key, T, Hash, KeyEqual, Allocator>;
    using const_iterator =
        unordered_map_const_iterator<Key, T, Hash, KeyEqual, Allocator>;

   private:
    using node_type = std::pair<const Key, T>;
    using bucket_type = std::list<node_type, Allocator>;

    std::vector<bucket_type> buckets;
    size_type element_count = 0;
    float max_load_factor_ = 1.0f;
    Hash hash_;
    KeyEqual key_eq_;
    Allocator alloc;

    size_t bucket_index(const Key& key) const {
        return hash_(key) % buckets.size();
    }

   public:
    // Constructors
    unordered_map() : unordered_map(16) {}

    explicit unordered_map(size_type bucket_count, const Hash& hash = Hash(),
                           const KeyEqual& equal = KeyEqual(),
                           const Allocator& alloc = Allocator())
        : buckets(bucket_count), hash_(hash), key_eq_(equal), alloc(alloc) {}

    unordered_map(const unordered_map& other) = default;
    unordered_map(unordered_map&& other) = default;

    // Destructor
    ~unordered_map() = default;

    // Assignment
    unordered_map& operator=(const unordered_map& other) = default;
    unordered_map& operator=(unordered_map&& other) = default;

    // Element access
    T& at(const Key& key) {
        auto it = find(key);
        if (it == end()) {
            throw std::out_of_range("Key not found");
        }
        return it->second;
    }

    const T& at(const Key& key) const {
        auto it = find(key);
        if (it == end()) {
            throw std::out_of_range("Key not found");
        }
        return it->second;
    }

    T& operator[](const Key& key) { return try_emplace(key).first->second; }

    T& operator[](Key&& key) {
        return try_emplace(std::move(key)).first->second;
    }

    // Iterators
    iterator begin() noexcept {
        auto vec_it = buckets.begin();
        while (vec_it != buckets.end() && vec_it->empty()) {
            ++vec_it;
        }
        return iterator(buckets.begin(), buckets.end(), vec_it,
                        (vec_it != buckets.end())
                            ? vec_it->begin()
                            : typename bucket_type::iterator());
    }

    const_iterator begin() const noexcept {
        auto vec_it = buckets.begin();
        while (vec_it != buckets.end() && vec_it->empty()) {
            ++vec_it;
        }
        return const_iterator(buckets.begin(), buckets.end(), vec_it,
                              (vec_it != buckets.end())
                                  ? vec_it->begin()
                                  : typename bucket_type::const_iterator());
    }

    iterator end() noexcept {
        return iterator(buckets.begin(), buckets.end(), buckets.end(),
                        typename bucket_type::iterator());
    }

    const_iterator end() const noexcept {
        return const_iterator(buckets.begin(), buckets.end(), buckets.end(),
                              typename bucket_type::const_iterator());
    }

    // Capacity
    bool empty() const noexcept { return element_count == 0; }
    size_type size() const noexcept { return element_count; }
    size_type max_size() const noexcept { return buckets.max_size(); }

    // Modifiers
    void clear() noexcept {
        for (auto& bucket : buckets) {
            bucket.clear();
        }
        element_count = 0;
    }

    std::pair<iterator, bool> insert(const value_type& value) {
        return emplace(value);
    }

    template <typename P>
    std::pair<iterator, bool> insert(P&& value) {
        return emplace(std::forward<P>(value));
    }

    template <typename... Args>
    std::pair<iterator, bool> emplace(Args&&... args) {
        if (load_factor() > max_load_factor_) {
            rehash(buckets.size() * 2);
        }

        node_type node(std::forward<Args>(args)...);
        size_t index = bucket_index(node.first);
        auto& bucket = buckets[index];

        for (auto it = bucket.begin(); it != bucket.end(); ++it) {
            if (key_eq_(it->first, node.first)) {
                return {iterator(buckets.begin(), buckets.end(),
                                 buckets.begin() + index, it),
                        false};
            }
        }

        bucket.push_front(std::move(node));
        ++element_count;
        return {iterator(buckets.begin(), buckets.end(),
                         buckets.begin() + index, bucket.begin()),
                true};
    }

    template <typename... Args>
    std::pair<iterator, bool> try_emplace(const Key& key, Args&&... args) {
        if (load_factor() > max_load_factor_) {
            rehash(buckets.size() * 2);
        }

        size_t index = bucket_index(key);
        auto& bucket = buckets[index];

        for (auto it = bucket.begin(); it != bucket.end(); ++it) {
            if (key_eq_(it->first, key)) {
                return {iterator(buckets.begin(), buckets.end(),
                                 buckets.begin() + index, it),
                        false};
            }
        }

        bucket.emplace_front(
            std::piecewise_construct, std::forward_as_tuple(key),
            std::forward_as_tuple(std::forward<Args>(args)...));
        ++element_count;
        return {iterator(buckets.begin(), buckets.end(),
                         buckets.begin() + index, bucket.begin()),
                true};
    }

    iterator erase(iterator pos) {
        if (pos == end())
            return end();

        size_t index = bucket_index(pos->first);
        buckets[index].erase(pos.bucket_it);
        --element_count;

        auto next = pos;
        ++next;
        return next;
    }

    size_type erase(const Key& key) {
        size_t index = bucket_index(key);
        auto& bucket = buckets[index];

        for (auto it = bucket.begin(); it != bucket.end(); ++it) {
            if (key_eq_(it->first, key)) {
                bucket.erase(it);
                --element_count;
                return 1;
            }
        }
        return 0;
    }

    // Lookup
    size_type count(const Key& key) const { return find(key) != end() ? 1 : 0; }

    iterator find(const Key& key) {
        size_t index = bucket_index(key);
        auto& bucket = buckets[index];

        for (auto it = bucket.begin(); it != bucket.end(); ++it) {
            if (key_eq_(it->first, key)) {
                return iterator(buckets.begin(), buckets.end(),
                                buckets.begin() + index, it);
            }
        }
        return end();
    }

    const_iterator find(const Key& key) const {
        size_t index = bucket_index(key);
        auto& bucket = buckets[index];

        for (auto it = bucket.begin(); it != bucket.end(); ++it) {
            if (key_eq_(it->first, key)) {
                return const_iterator(buckets.begin(), buckets.end(),
                                      buckets.begin() + index, it);
            }
        }
        return end();
    }

    bool contains(const Key& key) const { return find(key) != end(); }

    // Bucket interface
    size_type bucket_count() const noexcept { return buckets.size(); }

    size_type bucket_size(size_type n) const { return buckets[n].size(); }

    size_type bucket(const Key& key) const { return bucket_index(key); }


    float load_factor() const noexcept {
        return size() / static_cast<float>(bucket_count());
    }

    float max_load_factor() const noexcept { return max_load_factor_; }

    void max_load_factor(float ml) {
        max_load_factor_ = ml;
        if (load_factor() > max_load_factor_) {
            rehash(buckets.size() * 2);
        }
    }

    void rehash(size_type count) {
        if (count < size() / max_load_factor_) {
            count = std::ceil(size() / max_load_factor_);
        }

        std::vector<bucket_type> new_buckets(count);

        for (auto& bucket : buckets) {
            for (auto& node : bucket) {
                size_t index = hash_(node.first) % new_buckets.size();
                new_buckets[index].push_back(std::move(node));
            }
        }

        buckets = std::move(new_buckets);
    }

    void reserve(size_type count) {
        rehash(std::ceil(count / max_load_factor_));
    }

    // Observers
    Hash hash_function() const { return hash_; }

    KeyEqual key_eq() const { return key_eq_; }
};

#endif	// UNORDERED_MAP_H
