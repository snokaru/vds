#include <iostream>
#include <vector>
#include <string>
#include <stack>

#include <vds/DLList.hpp>
#include <vds/SLList.hpp>
#include <vds/CLList.hpp>
#include <vds/Stack.hpp>
#include <vds/Queue.hpp>

using namespace std;

int main() {
    vds::Queue<int> st;
    st.push(3);
    st.push(4);
    st.push(2);

    while (!st.empty()) {
        std::cout << st.front() << ' ';
        st.pop();
    }
    std::cout << '\n';
}
