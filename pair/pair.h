
#include <tuple>
#include <type_traits>
#include <utility>

#ifndef PAIR_H
#define PAIR_H

template <typename T1, typename T2>
struct pair {
    T1 first;
    T2 second;

    pair() = default;
    pair(const T1& a, const T2& b) : first(a), second(b) {}
    pair(T1&& a, T2&& b) : first(std::move(a)), second(std::move(b)) {}


    pair(const pair&) = default;
    pair(pair&&) = default;
    pair& operator=(const pair&) = default;
    pair& operator=(pair&&) = default;
};

#endif	// PAIR_H
