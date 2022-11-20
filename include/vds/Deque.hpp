#pragma once

#include "DLList.hpp"
#include <utility>
#include <cstddef>

namespace vds {

template <typename T>
class Deque {
public:
    using SizeType = size_t;
    void insert_front(T);
    void insert_back(T);
    void erase_front();
    void erase_back();
    const T& front() const;
    const T& back() const;
    SizeType size() const;
    bool empty() const;
private:
    SizeType sz;
    DLList<T> list;
};

template <typename T>
void Deque<T>::insert_front(T element) {
    sz++;
    list.push_front(std::move(element));
}

template <typename T>
void Deque<T>::insert_back(T element) {
    sz++;
    list.push_back(std::move(element));
}

template <typename T>
void Deque<T>::erase_front() {
    sz--;
    list.remove_front();
}

template <typename T>
void Deque<T>::erase_back() {
    sz--;
    list.remove_back();
}

template <typename T>
const T& Deque<T>::front() const {
    return list.front();
}

template <typename T>
const T& Deque<T>::back() const {
    return list.back();
}

template <typename T>
auto Deque<T>::size() const -> SizeType {
    return sz;
}

template <typename T>
bool Deque<T>::empty() const {
    return list.empty();
}


};