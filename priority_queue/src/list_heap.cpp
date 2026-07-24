#include "/media/kate/diskE/QT/LR8/task2/heap_list/include/list_heap.h"


void ListHeap::insert(int value){ //снизу вверх
    auto newNode = std::make_shared<HeapNode>(value);

    if(!root){
        root = newNode;
        nodes.push_back(newNode);
        return;
    }

    auto parentNode = nodes[(nodes.size() - 1) / 2].lock();

    if (!parentNode->left) {
        parentNode->left = newNode;
    } else {
        parentNode->right = newNode;
    }
    newNode->parent = parentNode;
    nodes.push_back(newNode);
    heapifyUp(newNode);

}

int ListHeap::extractMax(){
    if (isEmpty()) throw std::runtime_error("Heap is empty");
    int maxValue = root->value;
    auto lastNode = nodes.back().lock();

    if(root == lastNode){
        root.reset();
        nodes.pop_back();
        return maxValue;
    }
    root->value = lastNode ->value;
    auto parentNode = lastNode->parent.lock();
    if (parentNode->right == lastNode) {
        parentNode->right.reset();
    } else {
        parentNode->left.reset();
    }
    nodes.pop_back();
    heapifyDown(root);
    return maxValue;

}

bool ListHeap::isEmpty() const{
    return root == nullptr;
}

void ListHeap::heapifyUp(std::shared_ptr<HeapNode> node){ //Вставка, увеличение ключа (max-heap)
    while (node->parent.lock() && node->value > node->parent.lock()->value)
    {
        std::swap(node->value, node->parent.lock()->value);
        node = node->parent.lock();
    }
    
}



void ListHeap::heapifyDown(std::shared_ptr<HeapNode> node){ //Удаление, уменьшение ключа (max-heap)
    auto largest = node;
    if (node->left && node->left->value > largest->value) {
        largest = node->left;
    }
    if (node->right && node->right->value > largest->value) {
        largest = node->right;
    }
    if (largest != node) {
        std::swap(node->value, largest->value);
        heapifyDown(largest);
    }
}


void ListHeap::deleteKey(int i){
    if (i < 0 || i >= nodes.size()) {
        throw std::out_of_range("Index out of range");
    }
    auto nodeToDelete = nodes[i].lock();
    if (!nodeToDelete) return;
    auto lastNode = nodes.back().lock();
    if (!lastNode) return;

    nodeToDelete->value = lastNode->value;
    nodes.pop_back();

    if(auto parent = lastNode->parent.lock()){
        if (parent->left == lastNode) {
            parent->left.reset();
        } else if (parent->right == lastNode) {
            parent->right.reset();
        }
    }

    if (nodeToDelete->parent.lock() && 
     nodeToDelete->value > nodeToDelete->parent.lock()->value) {
    heapifyUp(nodeToDelete);
    } else {
        heapifyDown(nodeToDelete);
    }
}