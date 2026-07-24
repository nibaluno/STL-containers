#include "/media/kate/diskE/QT/LR8/task2/heap_array/include/array_heap.h"


void ArrayHeap::insert(int value){
    data.push_back(value);
    heapifyUp(data.size() - 1);
}

int ArrayHeap::extractMax(){
    if (isEmpty()) throw std::runtime_error("Heap is empty");
   int max = data[0];
   data[0] = data.back();
   data.pop_back();
   if (!isEmpty()) heapifyDown(0);
   return max;
}

void ArrayHeap::heapifyUp(size_t index) {

    while (index > 0 && data[index]  > data [parent(index)] ) {
        std:: swap (data[index]  , data [parent(index)]);
        index = parent(index);
    }
    
}

void ArrayHeap::heapifyDown(size_t index){
    size_t maxIndex = index;
    size_t left = leftChild(index);
    size_t right = rightChild(index);

    if(left < data.size() && data[maxIndex] < data[left]){
        maxIndex = left;
    }
    if(right < data.size() && data[maxIndex] < data[right]){
        maxIndex = right;
    }
    if (index != maxIndex) {
        std::swap(data[index], data[maxIndex]);
        heapifyDown(maxIndex);
    }

}
void ArrayHeap::deleteKey(int i) {
    if (i < 0 || i >= data.size()) {
        throw std::out_of_range("Index out of range");
    }
    data[i] = data.back();
    data.pop_back();
    if (i > 0 && data[i] > data[parent(i)]) {
        heapifyUp(i);
    } else {
        heapifyDown(i);
    }
}
size_t ArrayHeap::parent(size_t index) const {  return (index - 1) / 2; }

size_t ArrayHeap::leftChild(size_t index) const { return 2 * index + 1; }

size_t ArrayHeap::rightChild(size_t index) const { return 2 * index + 2; }

bool ArrayHeap::isEmpty() const { return data.empty(); }

size_t ArrayHeap::size() const { return data.size(); }

int ArrayHeap:: getMax() { return data[0]; }