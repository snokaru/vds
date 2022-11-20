cmake_minimum_required(VERSION 3.6)
project(vds)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# gtest
# include(FetchContent)
# FetchContent_Declare(
#   googletest
#   GIT_REPOSITORY https://github.com/google/googletest.git
#   GIT_TAG release-1.12.1
# )
# FetchContent_MakeAvailable(googletest)

# file(GLOB TESTS "test/*.cpp")
# enable_testing()
# add_executable(${PROJECT_NAME}-tests ${TESTS})
# target_include_directories(${PROJECT_NAME}-tests PUBLIC include)
# target_link_libraries(${PROJECT_NAME}-tests GTest::gtest_main gtest gmock)
# include(GoogleTest)
# gtest_discover_tests(${PROJECT_NAME}-tests)

set(INTERFACE 
  "include/${PROJECT_NAME}/SLList.hpp"
  "include/${PROJECT_NAME}/DLList.hpp"
  "include/${PROJECT_NAME}/CLList.hpp"
  "include/${PROJECT_NAME}/Recursion.hpp"
  "include/${PROJECT_NAME}/Stack.hpp"
)
set(SOURCES
  "src/main.cpp"
  "src/Recursion.cpp"
)
add_executable(${PROJECT_NAME} ${SOURCES})
target_include_directories(${PROJECT_NAME} PUBLIC include)

