#pragma once

#include <vector>
#include <algorithm>
#include <stdexcept>

class ArrayHeap
{

public: 
    ArrayHeap() = default;
    void insert(int value);
    int extractMax();
    bool isEmpty() const;
    size_t size() const;
    int getMax() ;
    void deleteKey(int i) ;
private:
    std::vector<int> data;
    void heapifyUp(size_t index);
    void heapifyDown(size_t index);
    size_t parent(size_t index) const;
    size_t leftChild(size_t index) const;
    size_t rightChild(size_t index) const;
};
