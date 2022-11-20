#include <iostream>
#include <vector>
#include <string>
#include <stack>

#include <vds/DLList.hpp>
#include <vds/SLList.hpp>
#include <vds/CLList.hpp>
#include <vds/Stack.hpp>
#include <vds/Queue.hpp>
#include <vds/Deque.hpp>

using namespace std;

int main() {
    vds::Deque<int> st;
    st.insert_front(3);
    st.insert_back(4);
    st.insert_front(2);

    while (!st.empty()) {
        std::cout << st.front() << ' ';
        st.erase_front();
    }
    std::cout << '\n';
}
