#pragma once

#include "SLList.hpp"

namespace vds {

template <typename T>
class Stack {
public:
    using SizeType = size_t;
    void push(T e);
    void pop();
    const T& top() const;
    bool empty() const;
    SizeType size() const;
private:
    vds::SLList<T> list;
    SizeType sz{0};
};

template <typename T>
void Stack<T>::push(T element) {
    list.push_front(std::move(element));
    sz++;
}

template <typename T>
void Stack<T>::pop() {
    list.remove_front();
    sz--;
}

template <typename T>
const T& Stack<T>::top() const {
    return list.front();
}

template <typename T>
bool Stack<T>::empty() const {
    return list.empty();
}

template <typename T>
auto Stack<T>::size() const -> SizeType {
    return sz;
}

} // namespace vds
