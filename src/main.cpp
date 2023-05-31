#include <iostream>
#include <stack>
#include <sys/wait.h>
#include <utility>
#include <vector>
#include <optional>
#include <ctime>

#include <vds/PriorityQueue.hpp>
#include <vds/OrderedArrayMap.hpp>
#include <vds/UnorderedHashMap.hpp>
#include <vds/OrderedSkipListMap.hpp>

int main(void) {
    srand (10);

    OrderedSkipListMap<std::string, int> map;
    if (map.empty()) {
        std::cout << "the map is empty...\n";
    } else {
        std::cout << "the map has some items...\n";
        std::cout << map.size() << " items.\n";
    }

    std::cout << "starting a loop over the map...\n";
    for (auto it = map.begin(); it != map.end(); ++it) {
        std::cout << "looping through the map...\n";
    }

    map.insert("test", 123);
    map.insert("abcd", 1234);
    std::cout << "LOOPING THROUGH MAP\n";
    for (auto it = map.begin(); it != map.end(); ++it) {
        std::cout << "looping through the map...\n";
        std::cout << "current item {" << it->first << ":" << it->second << "}\n";
    }

    std::cout << "CREATING COPY AND LOOPING THROUGH IT!\n";
    OrderedSkipListMap<std::string, int> copied_map = map;
    for (auto it = copied_map.begin(); it != copied_map.end(); ++it) {
        std::cout << "current item {" << it->first << ":" << it->second << "}\n";
    }

    std::cout << "EQUALS OPERATOR:\n";
    map = copied_map;
    for (auto it = map.begin(); it != map.end(); ++it) {
        std::cout << "current item {" << it->first << ":" << it->second << "}\n";
    }

    std::cout << "MOVING OPERATOR:\n";
    copied_map = std::move(map);
    for (auto it = copied_map.begin(); it != copied_map.end(); ++it) {
        std::cout << "current item {" << it->first << ":" << it->second << "}\n";
    }
}
