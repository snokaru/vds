#pragma once

#include <algorithm>
#include <vector>
#include <utility>

namespace vds {
template <typename Key, typename Value, typename Compare = std::less<Key>>
class OrderedArrayMap {
public:
    using Entry = std::pair<Key, Value>;
    using VectorIterator = typename std::vector<Entry>::iterator;
    using VectorSizeType = typename std::vector<Entry>::size_type;
    class Iterator {
    public:
        friend class OrderedArrayMap;

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

    OrderedArrayMap(Compare compare = Compare());

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
    Compare compare;
};

template <typename Key, typename Value, typename Compare>
OrderedArrayMap<Key, Value, Compare>::OrderedArrayMap(Compare compare)
: compare(std::move(compare))
{}

template <typename Key, typename Value, typename Compare>
OrderedArrayMap<Key, Value, Compare>::Iterator::Iterator(VectorIterator vit) 
:it(std::move(vit))
{}

template <typename Key, typename Value, typename Compare>
auto OrderedArrayMap<Key, Value, Compare>::Iterator::operator*() -> Entry& {
    return *it;
} 

template <typename Key, typename Value, typename Compare>
auto OrderedArrayMap<Key, Value, Compare>::Iterator::operator->() -> Entry* {
    return &(*it);
} 

template <typename Key, typename Value, typename Compare>
auto OrderedArrayMap<Key, Value, Compare>::Iterator::operator==(const Iterator& rhs) const -> bool {
    return it == rhs.it;
}

template <typename Key, typename Value, typename Compare>
auto OrderedArrayMap<Key, Value, Compare>::Iterator::operator!=(const Iterator& rhs) const -> bool {
    return !(it == rhs.it);
}

template <typename Key, typename Value, typename Compare>
auto OrderedArrayMap<Key, Value, Compare>::Iterator::operator++() -> Iterator& {
    ++it;
    return *this;
}

template <typename Key, typename Value, typename Compare>
auto OrderedArrayMap<Key, Value, Compare>::Iterator::operator++(int) -> Iterator {
    Iterator copy(*this);
    ++*this;
    return copy;
}

template <typename Key, typename Value, typename Compare>
auto OrderedArrayMap<Key, Value, Compare>::Iterator::operator--() -> Iterator& {
    --it;
    return *this;
}

template <typename Key, typename Value, typename Compare>
auto OrderedArrayMap<Key, Value, Compare>::Iterator::operator--(int) -> Iterator {
    Iterator copy(*this);
    --*this;
    return copy;
}

template <typename Key, typename Value, typename Compare>
auto OrderedArrayMap<Key, Value, Compare>::size() const -> VectorSizeType {
    return entries.size();
}

template <typename Key, typename Value, typename Compare>
auto OrderedArrayMap<Key, Value, Compare>::empty() const -> bool {
    return entries.empty();
}

template <typename Key, typename Value, typename Compare>
auto OrderedArrayMap<Key, Value, Compare>::begin() -> Iterator {
    return Iterator(entries.begin());
}

template <typename Key, typename Value, typename Compare>
auto OrderedArrayMap<Key, Value, Compare>::end() -> Iterator {
    return Iterator(entries.end());
}

template <typename Key, typename Value, typename Compare>
auto OrderedArrayMap<Key, Value, Compare>::find(const Key& key) const -> Iterator {
    auto it = begin();
    while (it != end() && it->first != key)
        it++;
    return it;
}

template <typename Key, typename Value, typename Compare>
auto OrderedArrayMap<Key, Value, Compare>::insert(Key key, Value value) -> Iterator {
    for (auto& entry : *this) {
        if (compare(entry->fist, key)) {
            entry = {std::move(key), std::move(value)};
            return;
        }
    }
    entries.push_back({std::move(key), std::move(value)});
}

template <typename Key, typename Value, typename Compare>
auto OrderedArrayMap<Key, Value, Compare>::erase(const Key& key) -> void {
    auto entryIterator = begin();
    while (entryIterator != end() && entryIterator->first != key)
        entryIterator++;
    
    if (entryIterator == end()) return;
    entries.erase(entryIterator.it);
}

template <typename Key, typename Value, typename Compare>
auto OrderedArrayMap<Key, Value, Compare>::erase(Iterator it) -> void {
    entries.erase(it.it);
}

template <typename Key, typename Value, typename Compare>
auto OrderedArrayMap<Key, Value, Compare>::operator[](Key&& key) -> Value& {
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

template <typename Key, typename Value, typename Compare>
typename vds::OrderedArrayMap<Key, Value, Compare>::Iterator 
begin(vds::OrderedArrayMap<Key, Value, Compare>& map) {
    return map.begin();
}

template <typename Key, typename Value, typename Compare>
typename vds::OrderedArrayMap<Key, Value, Compare>::Iterator 
end(vds::OrderedArrayMap<Key, Value, Compare>& map) {
    return map.end();
}
