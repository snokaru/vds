#pragma once

namespace vds {
template <typename T>
class DLList;

template <typename T>
class DLNode {
public:
    T element;
    DLNode* next;
    DLNode* previous;
};

template <typename T>
class DLList {
public:
    DLList();
    ~DLList();
    bool empty() const;
    const T& front() const;
    const T& back() const;
    void push_front(const T& element);
    void push_back(const T& element);
    void remove_front();
    void remove_back();
private:
    DLNode<T>* head;
    DLNode<T>* tail;
protected:
    void add(DLNode<T>* node, const T& element);
    void remove(DLNode<T>* node);
};

template <typename T>
DLList<T>::DLList() {
    head = new DLNode<T>;
    tail = new DLNode<T>;

    head->previous = nullptr;
    head->next = tail;

    tail->previous = head;
    tail->next = nullptr;
}

template <typename T>
DLList<T>::~DLList() {
    while(!empty()) remove_front();
    delete head;
    delete tail;
}

template <typename T>
bool DLList<T>::empty() const {
    return head->next == tail; 
}

template <typename T>
const T& DLList<T>::front() const {
    return head->next->element;
}

template <typename T>
const T& DLList<T>::back() const {
    return tail->previous->element;
}

template <typename T>
void DLList<T>::add(DLNode<T>* node, const T& element) {
    DLNode<T>* new_node = new DLNode<T>;
    new_node->element = element;
    new_node->next = node->next;
    new_node->previous = node;

    node->next->previous = new_node;
    node->next = new_node;
}

template <typename T>
void DLList<T>::push_back(const T& element) {
    add(head, element);
}

template <typename T>
void DLList<T>::push_front(const T& element) {
    add(tail->previous, element);
}

template <typename T>
void DLList<T>::remove(DLNode<T>* node) {
    node->next->previous = node->previous;
    node->previous->next = node->next;
    delete node;
}

template <typename T>
void DLList<T>::remove_front() {
    DLNode<T>* node_to_remove = head->next;
    head->next = head->next->next;
    head->next->previous = head;
    delete node_to_remove; 
}

template <typename T>
void DLList<T>::remove_back() {
    DLNode<T>* node_to_remove = tail->previous;
    tail->previous = tail->previous->previous;
    tail->previous->next = tail;
    delete node_to_remove;
}

}