#pragma once

#include <utility>

namespace vds {

template <typename T>
class SLList;

template <typename T>
void swap(SLList<T>&, SLList<T>&);

template <typename T>
struct SLNode {
    T element;
    SLNode* next;
};

template <typename T>
class SLList {
public:
    SLList();
    SLList(const SLList&);
    SLList(SLList&&);
    ~SLList();
    SLList& operator=(SLList);
    bool empty() const;
    const T& front() const;
    void push_front(T element);
    void remove_front();

    friend void swap<T>(SLList&, SLList&);
private:
    SLNode<T>* head;
};

template <typename T>
void swap(SLList<T>& lhs, SLList<T>& rhs) {
    using std::swap;
    swap(lhs.head, rhs.head);
}

template <typename T>
SLList<T>::SLList(const SLList<T>& other) {
    while (!empty()) {
        remove_front();
    }
    for (auto it = other.head->next; it != nullptr; it = it->next) {
        push_front(it->element);
    }
}

template <typename T>
SLList<T>::SLList(SLList<T>&& other)
:head(std::exchange(other.head, nullptr))
{}

template <typename T>
SLList<T>::SLList() {
    head = new SLNode<T>;
    head->next = nullptr;
}

template <typename T>
SLList<T>& SLList<T>::operator=(SLList<T> other) {
    swap(*this, other);
    return this;
}

template <typename T>
SLList<T>::~SLList() {
    while (!empty()) {
        remove_front();
    }
    delete head;
}

template <typename T>
bool SLList<T>::empty() const {
    return head->next == nullptr;
}

template <typename T>
const T& SLList<T>::front() const {
    return head->next->element;
}

template <typename T>
void SLList<T>::push_front(T element) {
    auto current_front = head->next;
    auto new_node = new SLNode<T>{std::move(element), current_front};
    head->next = new_node;
}

template <typename T>
void SLList<T>::remove_front() {
    auto current_front = head->next;
    head->next = head->next->next;
    delete current_front;
}

} // namespace vds
