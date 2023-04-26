#pragma once

#include <vector>
#include <variant>
#include <list>
#include <cstdlib>
#include <iostream>

template <typename Key, typename Value>
struct SkipListEntry {
    using Entry = std::pair<Key, Value>;

    Entry* entry{nullptr};

    SkipListEntry* next{nullptr};
    SkipListEntry* prev{nullptr};
    SkipListEntry* below{nullptr};
    SkipListEntry* above{nullptr};

    bool is_minus_inf();
    bool is_inf();
    bool is_top();
    bool is_bottom();

    Key& key();
    Value& value();
};

template <typename Key, typename Value>
auto SkipListEntry<Key, Value>::is_minus_inf() -> bool {
    return prev == nullptr;
}

template <typename Key, typename Value>
auto SkipListEntry<Key, Value>::is_inf() -> bool {
    return next == nullptr;
}

template <typename Key, typename Value>
auto SkipListEntry<Key, Value>::is_bottom() -> bool {
    return below == nullptr;
}

template <typename Key, typename Value>
auto SkipListEntry<Key, Value>::is_top() -> bool {
    return above == nullptr;
}

template <typename Key, typename Value>
auto SkipListEntry<Key, Value>::key() -> Key& {
    return entry->first;
}

template <typename Key, typename Value>
auto SkipListEntry<Key, Value>::value() -> Value& {
    return entry->second;
}

template <typename Key, typename Value, typename Compare = std::less<Key>>
class OrderedSkipListMap {
public:
    using Entry = SkipListEntry<Key, Value>;

    class Iterator {
    public:
        friend class OrderedSkipListMap;

        typename Entry::Entry& operator*();
        typename Entry::Entry* operator->();
        bool operator==(const Iterator&) const;
        bool operator!=(const Iterator&) const;
        Iterator& operator++();
        Iterator operator++(int);
        Iterator& operator--();
        Iterator operator--(int);
    private:
        Iterator(Entry*);
        Entry* current;
    };

    OrderedSkipListMap(Compare = Compare());
    ~OrderedSkipListMap();

    Iterator begin();
    Iterator end();

    size_t size() const;
    bool empty() const;
    Iterator find(const Key&) const;
    Iterator insert(Key, Value);
    void erase(const Key&);
    void erase(Iterator);
    Value& operator[](Key&& key);
private:
    // Object for managing ownership of entries in order to avoid unnecessary
    // duplication in upper layers of the skip list.
    std::list<typename Entry::Entry> entries;

    Compare less;
    Entry* top_left;
    Entry* top_right;
    Entry* bottom_left;
    Entry* bottom_right;

    Entry* _find_after(const Key& key) const; 
    void clear();
};

template <typename Key, typename Value, typename Compare>
OrderedSkipListMap<Key, Value, Compare>::Iterator::Iterator(Entry* current)
: current{current} {}

template <typename Key, typename Value, typename Compare>
auto OrderedSkipListMap<Key, Value, Compare>::Iterator::operator*() -> typename Entry::Entry& {
    return *current->entry;
}

template <typename Key, typename Value, typename Compare>
auto OrderedSkipListMap<Key, Value, Compare>::Iterator::operator->() -> typename Entry::Entry* {
    return current->entry;
}

template <typename Key, typename Value, typename Compare>
auto OrderedSkipListMap<Key, Value, Compare>::Iterator::operator==(const Iterator& other) const -> bool {
    return current == other.current;
} 

template <typename Key, typename Value, typename Compare>
auto OrderedSkipListMap<Key, Value, Compare>::Iterator::operator!=(const Iterator& other) const -> bool {
    return !(*this == other);
} 

template <typename Key, typename Value, typename Compare>
auto OrderedSkipListMap<Key, Value, Compare>::Iterator::operator++() -> Iterator& {
    current = current->next;
    return *this;
}

template <typename Key, typename Value, typename Compare>
auto OrderedSkipListMap<Key, Value, Compare>::Iterator::operator++(int) -> Iterator {
    Iterator prev(*this);
    ++this;
    return prev;
}

template <typename Key, typename Value, typename Compare>
auto OrderedSkipListMap<Key, Value, Compare>::Iterator::operator--() -> Iterator& {
    current = current->prev;
    return *this;
}

template <typename Key, typename Value, typename Compare>
auto OrderedSkipListMap<Key, Value, Compare>::Iterator::operator--(int) -> Iterator {
    Iterator prev(*this);
    --this;
    return prev;
}

template <typename Key, typename Value, typename Compare>
OrderedSkipListMap<Key, Value, Compare>::OrderedSkipListMap(Compare compare)
: entries({{}, {}})
, less(std::move(compare))
, top_left(new Entry())
, top_right(new Entry())
, bottom_left(top_left)
, bottom_right(top_right) {
    top_left->entry = &entries.front();
    bottom_left->entry = &entries.back();
    top_left->next = top_right;
    top_right->prev = top_left;
}

template <typename Key, typename Value, typename Compare>
auto OrderedSkipListMap<Key, Value, Compare>::begin() -> Iterator {
    return Iterator(bottom_left->next);
}

template <typename Key, typename Value, typename Compare>
auto OrderedSkipListMap<Key, Value, Compare>::end() -> Iterator {
    return Iterator(bottom_right);
}

template <typename Key, typename Value, typename Compare>
auto OrderedSkipListMap<Key, Value, Compare>::size() const -> size_t {
    return entries.size() - 2;
}

template <typename Key, typename Value, typename Compare>
auto OrderedSkipListMap<Key, Value, Compare>::empty() const -> bool {
    return size() == 0;
}

template <typename Key, typename Value, typename Compare>
auto OrderedSkipListMap<Key, Value, Compare>::_find_after(const Key& key) const -> Entry* {
    auto it = top_left;

    do {
        while (it->is_minus_inf() or (not it->is_inf() and less(key, it->key()))) {
            it = it->next;
        }
        it = it->is_bottom() ? it : it->below;
    } while (!it->is_bottom());

    return it;
}

template <typename Key, typename Value, typename Compare>
auto OrderedSkipListMap<Key, Value, Compare>::find(const Key& key) const -> Iterator {
    auto after_key_ptr = _find_after(key);
    return Iterator(after_key_ptr);
}

template <typename Key, typename Value, typename Compare>
auto OrderedSkipListMap<Key, Value, Compare>::insert(Key key, Value value) -> Iterator {
    std::cout << "inserting!\n";
    auto after_key_ptr = _find_after(key);

    if (not after_key_ptr->is_inf() and not less(after_key_ptr->key(), key)) {
        // TODO: notify callee that no insertion actually happened
        return Iterator(after_key_ptr);
    }

    entries.push_back({std::move(key), std::move(value)});
    auto new_entry_ptr = new Entry();
    new_entry_ptr->entry = &entries.back();
    new_entry_ptr->next = after_key_ptr;
    new_entry_ptr->prev = after_key_ptr->prev;

    after_key_ptr->prev->next = new_entry_ptr;
    after_key_ptr->prev = new_entry_ptr;

    auto left_ptr = bottom_left, right_ptr = bottom_right;
    auto new_entry_current_level_ptr = new_entry_ptr;
    while (rand() % 2 == 0) {
        std::cout << "going above...\n";
        if (left_ptr->above == nullptr) {
            // new layer should be added
            auto new_left_ptr = new Entry();
            new_left_ptr->below = left_ptr;
            new_left_ptr->entry = &entries.back();
            left_ptr->above = new_left_ptr;

            auto new_right_ptr = new Entry();
            new_right_ptr->below = right_ptr;
            new_right_ptr->entry = &*(++entries.begin());
            right_ptr->above = new_right_ptr;

            new_left_ptr->next = new_right_ptr;
            new_right_ptr->prev = new_left_ptr;

            top_left = new_left_ptr;
            top_right = new_right_ptr;
        }

        auto new_entry_above_ptr = new Entry();
        new_entry_above_ptr->entry = &entries.back();

        auto closest_above_left = new_entry_current_level_ptr->prev;
        while (not closest_above_left->above) closest_above_left = closest_above_left->prev;
        closest_above_left = closest_above_left->above;

        auto closest_above_right = new_entry_current_level_ptr->next;
        while (not closest_above_right->above) closest_above_right = closest_above_right->next;
        closest_above_right = closest_above_right->above;

        closest_above_left->next = new_entry_above_ptr;
        closest_above_right->prev = new_entry_above_ptr;

        new_entry_above_ptr->next = closest_above_right;
        new_entry_above_ptr->prev = closest_above_left;

        new_entry_current_level_ptr->above = new_entry_above_ptr;
        
        new_entry_current_level_ptr = new_entry_above_ptr;
        left_ptr = left_ptr->above;
        right_ptr = right_ptr->above;
    }

    return Iterator(new_entry_ptr);
}

template <typename Key, typename Value, typename Compare>
auto OrderedSkipListMap<Key, Value, Compare>::clear() -> void {
    auto down_moving_ptr = top_left;
    while (down_moving_ptr) {
        auto below = down_moving_ptr->below;

        auto right_moving_ptr = down_moving_ptr;
        while (right_moving_ptr) {
            std::cout << "deleting one node..\n";
            auto next = right_moving_ptr->next;
            delete right_moving_ptr;
            right_moving_ptr = next;
        }

        std::cout << "going below...\n";
        down_moving_ptr = below;
    }
}

template <typename Key, typename Value, typename Compare>
OrderedSkipListMap<Key, Value, Compare>::~OrderedSkipListMap() {
    std::cout << "destructing...\n";
    clear();
}