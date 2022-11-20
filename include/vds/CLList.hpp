#pragma once

#include <utility>
#include <iostream>

namespace vds {
template <typename T>
struct CLNode {
    T element;
    CLNode* next;
};

template <typename T>
class CLList {
public:
    ~CLList();
    bool empty() const;
    const T& front() const;
    const T& back() const;
    void add(T element);
    void remove();
    void advance();
private:
    CLNode<T>* cursor = nullptr;
};

template <typename T>
CLList<T>::~CLList() {
    while (!empty()) {
        remove();
    }
}

template <typename T>
bool CLList<T>::empty() const {
    return cursor == nullptr;
}

template <typename T>
const T& CLList<T>::front() const {
    return cursor->next->element;
}

template <typename T>
const T& CLList<T>::back() const {
    return cursor->element;
}

template <typename T>
void CLList<T>::add(T element) {
    if (!cursor) {
        cursor = new CLNode<T>{std::move(element), nullptr};
        cursor->next = cursor;
        return;
    }
    auto current_after_cursor = cursor->next;
    auto new_node = new CLNode<T>{std::move(element), current_after_cursor};
    cursor->next = new_node;
}

template <typename T>
void CLList<T>::remove() {
    if (cursor == cursor->next) {
        delete cursor;
        cursor = nullptr;
        return;
    }
    auto current_front = cursor->next;
    cursor->next = cursor->next->next;
    delete current_front;
}

template <typename T>
void CLList<T>::advance() {
    cursor = cursor->next;
}

} // namespace vds
