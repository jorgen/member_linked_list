add_executable(simple_test simple_test.cpp)
target_link_libraries(simple_test PRIVATE MemberLinkedList)
include_directories(simple_test ${LinkedList_SOURCE_DIR}/include)
