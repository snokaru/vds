#pragma once

#include <cstddef>
#include "CLList.hpp"

namespace vds {

template <typename T>
class Queue {
public:
    using SizeType = size_t;
    void push(T e);
    void pop();
    const T& front() const;
    bool empty() const;
    SizeType size() const;
private:
    vds::CLList<T> list;
    SizeType sz{0};
};

template <typename T>
void Queue<T>::push(T element) {
    sz++;
    list.add(std::move(element));
}

template <typename T>
void Queue<T>::pop() {
    sz--;
    list.remove();
}

template <typename T>
const T& Queue<T>::front() const {
    return list.front();
}

template <typename T>
bool Queue<T>::empty() const {
    return list.empty();
}

template <typename T>
auto Queue<T>::size() const -> SizeType {
    return sz;
}

} // namespace vds