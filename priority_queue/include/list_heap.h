#pragma once

#include <memory>
#include <vector>
#include <algorithm>
#include <stdexcept>

class HeapNode
{

public:
    int value;
    std::shared_ptr<HeapNode> left; 
    std::shared_ptr<HeapNode> right;
    std::weak_ptr<HeapNode> parent;
    explicit HeapNode(int val) : value(val) {}
};


class ListHeap
{
public:
    ListHeap() = default;
    void insert(int value);
    int extractMax();
    bool isEmpty() const;

int getMax() const {
    if (isEmpty()) throw std::runtime_error("Heap is empty");
    return root->value;
}
void deleteKey(int i);
size_t size() const {
    return nodes.size();
}
private:
    std::shared_ptr<HeapNode> root;
    std::vector<std::weak_ptr<HeapNode>> nodes;
    void heapifyUp(std::shared_ptr<HeapNode> node);
    void heapifyDown(std::shared_ptr<HeapNode> node);
};


