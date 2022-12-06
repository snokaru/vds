#pragma once

#include <algorithm>
#include <vector>
#include <utility>

namespace vds {
template <typename Key, typename Value>
class ArrayOrderedMap {
public:
    using Entry = std::pair<Key, Value>;
    using VectorIterator = typename std::vector<Entry>::iterator;
    using VectorSizeType = typename std::vector<Entry>::size_type;

    class Iterator {
    public:
        friend class ArrayOrderedMap;

        Entry& operator*();
        Entry* operator->();
        bool operator==(const Iterator&) const;
        bool operator!=(const Iterator&) const;
        Iterator& operator++();
        Iterator operator++(int);
        Iterator& operator--();
        Iterator operator--(int);
    private:
        Iterator() = default;
        Iterator(VectorIterator);
        VectorIterator it;
    };


    Iterator begin();
    Iterator end();

    VectorSizeType size() const;
    bool empty() const;
    Iterator find(const Key&) const;
    Iterator insert(Key, Value);
    void erase(const Key&);
    void erase(Iterator);
    Value& operator[](Key&& key);
private:
    std::vector<Entry> entries;
};

template<typename Key, typename Value>
ArrayOrderedMap<Key, Value>::Iterator::Iterator(VectorIterator vit) 
:it(std::move(vit))
{}

template<typename Key, typename Value>
auto ArrayOrderedMap<Key, Value>::Iterator::operator*() -> Entry& {
    return *it;
} 

template<typename Key, typename Value>
auto ArrayOrderedMap<Key, Value>::Iterator::operator->() -> Entry* {
    return &(*it);
} 

template<typename Key, typename Value>
auto ArrayOrderedMap<Key, Value>::Iterator::operator==(const Iterator& rhs) const -> bool {
    return it == rhs.it;
}

template<typename Key, typename Value>
auto ArrayOrderedMap<Key, Value>::Iterator::operator!=(const Iterator& rhs) const -> bool {
    return !(it == rhs.it);
}

template<typename Key, typename Value>
auto ArrayOrderedMap<Key, Value>::Iterator::operator++() -> Iterator& {
    ++it;
    return *this;
}

template<typename Key, typename Value>
auto ArrayOrderedMap<Key, Value>::Iterator::operator++(int) -> Iterator {
    Iterator copy(*this);
    ++*this;
    return copy;
}

template<typename Key, typename Value>
auto ArrayOrderedMap<Key, Value>::Iterator::operator--() -> Iterator& {
    --it;
    return *this;
}

template<typename Key, typename Value>
auto ArrayOrderedMap<Key, Value>::Iterator::operator--(int) -> Iterator {
    Iterator copy(*this);
    --*this;
    return copy;
}

template <typename Key, typename Value>
auto ArrayOrderedMap<Key, Value>::size() const -> VectorSizeType {
    return entries.size();
}

template<typename Key, typename Value>
auto ArrayOrderedMap<Key, Value>::empty() const -> bool {
    return entries.empty();
}

template <typename Key, typename Value>
auto ArrayOrderedMap<Key, Value>::begin() -> Iterator {
    return Iterator(entries.begin());
}

template <typename Key, typename Value>
auto ArrayOrderedMap<Key, Value>::end() -> Iterator {
    return Iterator(entries.end());
}

template <typename Key, typename Value>
auto ArrayOrderedMap<Key, Value>::find(const Key& key) const -> Iterator {
    auto it = begin();
    while (it != end() && it->first != key)
        it++;
    return it;
}

template <typename Key, typename Value>
auto ArrayOrderedMap<Key, Value>::insert(Key key, Value value) -> Iterator {
    for (auto& entry : *this) {
        if (entry->first == key) {
            entry = {std::move(key), std::move(value)};
            return;
        }
    }
    entries.push_back({std::move(key), std::move(value)});
}

template <typename Key, typename Value>
auto ArrayOrderedMap<Key, Value>::erase(const Key& key) -> void {
    auto entryIterator = begin();
    while (entryIterator != end() && entryIterator->first != key)
        entryIterator++;
    
    if (entryIterator == end()) return;
    entries.erase(entryIterator.it);
}

template <typename Key, typename Value>
auto ArrayOrderedMap<Key, Value>::erase(Iterator it) -> void {
    entries.erase(it.it);
}

template <typename Key, typename Value>
auto ArrayOrderedMap<Key, Value>::operator[](Key&& key) -> Value& {
    auto entryIterator = begin();
    while(entryIterator != end() && entryIterator->first != key)
        entryIterator++;
    if (entryIterator != end()) {
        return entryIterator->second;
    }
    entries.push_back({std::forward<Key>(key), Value()});
    return entries.back().second;
}
}

template<typename Key, typename Value>
typename vds::ArrayOrderedMap<Key, Value>::Iterator 
begin(vds::ArrayOrderedMap<Key, Value>& map) {
    return map.begin();
}

template<typename Key, typename Value>
typename vds::ArrayOrderedMap<Key, Value>::Iterator 
end(vds::ArrayOrderedMap<Key, Value>& map) {
    return map.end();
}



