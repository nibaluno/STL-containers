#ifndef UNMAPITERATOR_H
#define UNMAPITERATOR_H


#include <functional>
#include <list>
#include <memory>
#include <stdexcept>
#include <vector>

template <typename Key, typename T, typename Hash, typename KeyEqual,
          typename Allocator>
class unordered_map;

template <typename Key, typename T, typename Hash, typename KeyEqual,
          typename Allocator>
class unordered_map_iterator {
   public:
    using iterator_category = std::forward_iterator_tag;
    using value_type = std::pair<const Key, T>;
    using difference_type = ptrdiff_t;
    using pointer = value_type*;
    using reference = value_type&;

   private:
    using bucket_type = std::list<std::pair<const Key, T>, Allocator>;
    using bucket_iterator = typename bucket_type::iterator;
    using vector_iterator = typename std::vector<bucket_type>::iterator;

    vector_iterator vec_it;
    vector_iterator vec_end;
    bucket_iterator bucket_it;

   public:
    unordered_map_iterator(vector_iterator begin, vector_iterator end,
                           vector_iterator current, bucket_iterator bucket)
        : vec_it(current), vec_end(end), bucket_it(bucket) {
        if (vec_it != vec_end && bucket_it == vec_it->end()) {
            ++*this;
        }
    }

    reference operator*() const { return *bucket_it; }
    pointer operator->() const { return &*bucket_it; }

    unordered_map_iterator& operator++() {
        if (vec_it == vec_end)
            return *this;

        ++bucket_it;
        while (bucket_it == vec_it->end()) {
            if (++vec_it == vec_end)
                break;
            bucket_it = vec_it->begin();
        }
        return *this;
    }

    unordered_map_iterator operator++(int) {
        unordered_map_iterator tmp = *this;
        ++*this;
        return tmp;
    }

    bool operator==(const unordered_map_iterator& other) const {
        return vec_it == other.vec_it &&
               (vec_it == vec_end || bucket_it == other.bucket_it);
    }

    bool operator!=(const unordered_map_iterator& other) const {
        return !(*this == other);
    }

    friend class unordered_map<Key, T, Hash, KeyEqual, Allocator>;
};

template <typename Key, typename T, typename Hash, typename KeyEqual,
          typename Allocator>
class unordered_map_const_iterator {
   public:
    using iterator_category = std::forward_iterator_tag;
    using value_type = const std::pair<const Key, T>;
    using difference_type = ptrdiff_t;
    using pointer = const value_type*;
    using reference = const value_type&;

   private:
    using bucket_type = std::list<std::pair<const Key, T>, Allocator>;
    using bucket_iterator = typename bucket_type::const_iterator;
    using vector_iterator = typename std::vector<bucket_type>::const_iterator;

    vector_iterator vec_it;
    vector_iterator vec_end;
    bucket_iterator bucket_it;

   public:
    unordered_map_const_iterator(vector_iterator begin, vector_iterator end,
                                 vector_iterator current,
                                 bucket_iterator bucket)
        : vec_it(current), vec_end(end), bucket_it(bucket) {
        if (vec_it != vec_end && bucket_it == vec_it->end()) {
            ++*this;
        }
    }

    unordered_map_const_iterator(
        const unordered_map_iterator<Key, T, Hash, KeyEqual, Allocator>& it)
        : vec_it(it.vec_it), vec_end(it.vec_end), bucket_it(it.bucket_it) {}

    reference operator*() const { return *bucket_it; }
    pointer operator->() const { return &*bucket_it; }

    unordered_map_const_iterator& operator++() {
        if (vec_it == vec_end)
            return *this;

        ++bucket_it;
        while (bucket_it == vec_it->end()) {
            if (++vec_it == vec_end)
                break;
            bucket_it = vec_it->begin();
        }
        return *this;
    }

    unordered_map_const_iterator operator++(int) {
        unordered_map_const_iterator tmp = *this;
        ++*this;
        return tmp;
    }

    bool operator==(const unordered_map_const_iterator& other) const {
        return vec_it == other.vec_it &&
               (vec_it == vec_end || bucket_it == other.bucket_it);
    }

    bool operator!=(const unordered_map_const_iterator& other) const {
        return !(*this == other);
    }
};

#endif	// UNMAPITERATOR_H
