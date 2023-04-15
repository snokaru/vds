#pragma once

#include <algorithm>
#include <vector>
#include <list>
#include <utility>

namespace vds {
template <typename Key, typename Value, typename Hash = std::hash<Key>, typename Equals = std::equal_to<Key>>
class UnorderedHashMap {
public:
    using Entry = std::pair<Key, Value>;
    using Bucket = std::list<Entry>;
    using BucketIterator = typename std::vector<Bucket>::iterator;
    using EntryIterator = typename Bucket::iterator;
    using VectorSizeType = typename std::vector<Bucket>::size_type;

    UnorderedHashMap(
        VectorSizeType buckets_count = 100,
        Hash hash = Hash(),
        Equals equals = Equals());

    class Iterator {
    public:
        friend class UnorderedHashMap;

        Entry& operator*();
        Entry* operator->();
        bool operator==(const Iterator&) const;
        bool operator!=(const Iterator&) const;
        Iterator& operator++();
        Iterator operator++(int);
    private:
        Iterator() = default;
        Iterator(
            const std::vector<Bucket>&,
            BucketIterator, 
            EntryIterator);

        const std::vector<Bucket>* buckets_ptr;
        BucketIterator bucket_it;
        EntryIterator entry_it;
    };


    Iterator begin();
    Iterator end();

    VectorSizeType size();
    bool empty();
    Iterator find(const Key&) const;
    Iterator insert(Key, Value);
    void erase(const Key&);
    void erase(Iterator);
    Value& operator[](Key&& key);
private:
    std::vector<Bucket> buckets;
    Hash hash;
    Equals equals;
};

template <typename Key, typename Value, typename Hash, typename Equals>
UnorderedHashMap<Key, Value, Hash, Equals>::Iterator::Iterator(
    const std::vector<Bucket>& buckets_ptr,
    BucketIterator bucket_it,
    EntryIterator entry_it)
: buckets_ptr(&buckets_ptr)
, bucket_it(bucket_it)
, entry_it(entry_it)
{}

template <typename Key, typename Value, typename Hash, typename Equals>
auto UnorderedHashMap<Key, Value, Hash, Equals>::Iterator::operator*() -> Entry& {
    return *entry_it;
}

template <typename Key, typename Value, typename Hash, typename Equals>
auto UnorderedHashMap<Key, Value, Hash, Equals>::Iterator::operator->() -> Entry* {
    return &(*entry_it);
}

template <typename Key, typename Value, typename Hash, typename Equals>
auto UnorderedHashMap<Key, Value, Hash, Equals>::Iterator::operator==(const Iterator& other) const -> bool {
    return 
        (bucket_it == other.bucket_it and entry_it == other.entry_it) || 
        (bucket_it == buckets_ptr->end() and other.bucket_it == buckets_ptr->end());
}

template <typename Key, typename Value, typename Hash, typename Equals>
auto UnorderedHashMap<Key, Value, Hash, Equals>::Iterator::operator!=(const Iterator& other) const -> bool {
    return !(*this == other);
}

template <typename Key, typename Value, typename Hash, typename Equals>
auto UnorderedHashMap<Key, Value, Hash, Equals>::Iterator::operator++() -> Iterator& {
    entry_it++;
    if (entry_it != bucket_it->end())
        return *this;

    do
    {
        bucket_it++;
    } while(bucket_it->empty() and bucket_it != buckets_ptr->end());

    if (bucket_it != buckets_ptr->end())
        entry_it = bucket_it->begin();
    
    return *this;
}

template <typename Key, typename Value, typename Hash, typename Equals>
auto UnorderedHashMap<Key, Value, Hash, Equals>::Iterator::operator++(int) -> Iterator {
    Iterator copy(*this);
    ++this;
    return copy;
}
    
template <typename Key, typename Value, typename Hash, typename Equals>
UnorderedHashMap<Key, Value, Hash, Equals>::UnorderedHashMap(
    VectorSizeType bucket_count,
    Hash hash,
    Equals equals)
: buckets(bucket_count)
, hash(std::move(hash))
, equals(std::move(equals))
{}

template <typename Key, typename Value, typename Hash, typename Equals>
bool UnorderedHashMap<Key, Value, Hash, Equals>::empty() {
    return begin() == end();
}

template <typename Key, typename Value, typename Hash, typename Equals>
auto UnorderedHashMap<Key, Value, Hash, Equals>::size() -> VectorSizeType {
    return std::distance(begin(), end());
}

template <typename Key, typename Value, typename Hash, typename Equals>
auto UnorderedHashMap<Key, Value, Hash, Equals>::begin() -> Iterator {
    BucketIterator bucket_it = buckets.begin();
    while (bucket_it->empty() && bucket_it != buckets.end()) ++bucket_it;
    if (bucket_it == buckets.end())
        return Iterator(buckets, bucket_it, EntryIterator());
    return Iterator(buckets, bucket_it, bucket_it->begin());
}

template <typename Key, typename Value, typename Hash, typename Equals>
auto UnorderedHashMap<Key, Value, Hash, Equals>::end() -> Iterator {
    return Iterator(buckets, buckets.end(), EntryIterator());
}

template <typename Key, typename Value, typename Hash, typename Equals>
auto UnorderedHashMap<Key, Value, Hash, Equals>::find(const Key& key) const -> Iterator {
    auto location = hash(key) % buckets.size();
    BucketIterator bucket_it = buckets.begin() + location;
    for (auto entry_it = bucket_it->begin(); entry_it != bucket_it->end(); entry_it++) {
        if (equals(entry_it->first, key))
            return Iterator(buckets, bucket_it, entry_it);
    }
    return end();
}

template <typename Key, typename Value, typename Hash, typename Equals>
auto UnorderedHashMap<Key, Value, Hash, Equals>::operator[](Key&& key) -> Value& {
    auto location = hash(key) % buckets.size();
    BucketIterator bucket_it = buckets.begin() + location;
    for (auto entry_it = bucket_it->begin(); entry_it != bucket_it->end(); entry_it++) {
        if (equals(entry_it->first, key))
            return entry_it->second;
    }
    bucket_it->push_back({std::forward<Key>(key), Value()});
    return bucket_it->back().second;
}

template <typename Key, typename Value, typename Hash, typename Equals>
auto UnorderedHashMap<Key, Value, Hash, Equals>::insert(Key key, Value value) -> Iterator {
    auto location = hash(key) % buckets.size();
    BucketIterator bucket_it = buckets.begin() + location;
    for (auto entry_it = bucket_it->begin(); entry_it != bucket_it->end(); entry_it++) {
        if (equals(entry_it->first, key))
            return Iterator(buckets, bucket_it, entry_it);
    }
    bucket_it->push_back({std::move(key), std::move(value)});
    return Iterator(buckets, bucket_it, --bucket_it->end());
}

template <typename Key, typename Value, typename Hash, typename Equals>
auto UnorderedHashMap<Key, Value, Hash, Equals>::erase(const Key& key) -> void {
    auto location = hash(key) % buckets.size();
    BucketIterator bucket_it = buckets.begin() + location;
    for (auto entry_it = bucket_it->begin(); entry_it != bucket_it->end(); entry_it++) {
        if (equals(entry_it->first, key))
            bucket_it->erase(entry_it);
    }
}

template <typename Key, typename Value, typename Hash, typename Equals>
auto UnorderedHashMap<Key, Value, Hash, Equals>::erase(Iterator it) -> void {
    it.bucket_it->erase(it.entry_it);
}

}