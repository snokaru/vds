#include <iostream>
#include <stack>
#include <utility>
#include <vector>
#include <optional>

#include <vds/PriorityQueue.hpp>
#include <vds/OrderedArrayMap.hpp>
#include <vds/UnorderedHashMap.hpp>

int main(void) {
    vds::UnorderedHashMap<std::string, int> myMap;

    myMap["a"] = 754;
    myMap["b"] = 123;
    myMap["c"] = 1394;
    auto it = myMap.insert("testing", 344);
    std::cout << "inserted with insert: " << it->first << '\n';
    for (const auto& el: myMap) {
        std::cout << el.first << ": " << el.second << '\n';
    }

    myMap.erase(it);
    for (const auto& el: myMap) {
        std::cout << el.first << ": " << el.second << '\n';
    }
}
