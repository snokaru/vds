#pragma once

#include <vector>
#include <utility>
#include <iostream>

namespace vds {

template <typename T, typename Compare = std::less<typename std::vector<T>::value_type>>
class PriorityQueue {
public:
    PriorityQueue(Compare = Compare());
    void insert(T item);
    const T& min() const;
    void removeMin(); 
    bool empty() const;
private:
    int parent(int);
    int leftChild(int);
    int rightChild(int);


    Compare isLess;
    std::vector<T> heap;
};

template <typename T, typename Compare>
PriorityQueue<T, Compare>::PriorityQueue(Compare isLess)
: isLess(std::move(isLess)) {}

template <typename T, typename Compare>
int PriorityQueue<T, Compare>::parent(int idx) {
    return (idx - 1) / 2;
}

template <typename T, typename Compare>
int PriorityQueue<T, Compare>::leftChild(int idx) {
    return idx * 2 + 1;
}

template <typename T, typename Compare>
int PriorityQueue<T, Compare>::rightChild(int idx) {
    return idx * 2 + 2;
}

template <typename T, typename Compare>
void PriorityQueue<T, Compare>::insert(T item) {
    heap.push_back(std::move(item));
    auto element_pos = heap.size() - 1;
    while (parent(element_pos) >= 0 && isLess(heap[element_pos], heap[parent(element_pos)])) {
        std::cout << "bubbling up...\n";
        swap(heap[element_pos], heap[parent(element_pos)]);
        element_pos = parent(element_pos);
    }
}

template <typename T, typename Compare>
bool PriorityQueue<T, Compare>::empty() const {
    return heap.empty();
}

template <typename T, typename Compare>
const T& PriorityQueue<T, Compare>::min() const {
    return heap.front();
}

template <typename T, typename Compare>
void PriorityQueue<T, Compare>::removeMin() {
    swap(heap.front(), heap.back());
    heap.pop_back();
    int pos = 0;
    while ((leftChild(pos) < heap.size() && isLess(heap[leftChild(pos)], heap[pos])) ||
           (rightChild(pos) < heap.size() && isLess(heap[rightChild(pos)], heap[pos]))) {
        if (leftChild(pos) < heap.size() && isLess(heap[leftChild(pos)], heap[pos])) {
            swap(heap[pos], heap[leftChild(pos)]);
            pos = leftChild(pos);
        } else if (rightChild(pos) < heap.size() && isLess(heap[rightChild(pos)], heap[pos])) {
            swap(heap[pos], heap[rightChild(pos)]);
            pos = rightChild(pos);
        }
    }
}

}
