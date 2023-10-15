#include <catch2/catch_test_macros.hpp>

#include <MemberLinkedList.h>

class MyClass
{
public:
    MyClass(int value) : value(value) {}
    int value;
    MemberListNode<MyClass> listNode;
};

TEST_CASE("MemberList operations", "[MemberList]")
{
    MemberList<MyClass, &MyClass::listNode> myList;

    SECTION("Initial state") {
        REQUIRE(myList.first() == nullptr);
        REQUIRE(myList.last() == nullptr);
    }

    SECTION("push_front and iteration") {
        MyClass a(1), b(2), c(3);
        myList.push_front(&a);
        myList.push_front(&b);
        myList.push_front(&c);

        int expected[] = {3, 2, 1};
        int i = 0;
        for (auto it = myList.begin(); it != myList.end(); ++it, ++i) {
            REQUIRE(it->value == expected[i]);
        }
    }

    SECTION("pop_front") {
        MyClass a(1), b(2);
        myList.push_front(&a);
        myList.push_front(&b);

        myList.pop_front();
        
        REQUIRE(myList.first()->value == 1);
        REQUIRE(myList.last()->value == 1);
    }

    SECTION("push_back") {
        MyClass a(1), b(2), c(3);
        myList.push_back(&a);
        myList.push_back(&b);
        myList.push_back(&c);

        int expected[] = {1, 2, 3};
        int i = 0;
        for (auto it = myList.begin(); it != myList.end(); ++it, ++i) {
            REQUIRE(it->value == expected[i]);
        }
    }

    SECTION("pop_back") {
        MyClass a(1), b(2), c(3);
        myList.push_back(&a);
        myList.push_back(&b);
        myList.push_back(&c);

        myList.pop_back();

        int expected[] = {1, 2};
        int i = 0;
        for (auto it = myList.begin(); it != myList.end(); ++it, ++i) {
            REQUIRE(it->value == expected[i]);
        }
    }

    SECTION("insert") {
        MyClass a(1), b(3);
        myList.push_back(&a);
        myList.push_back(&b);

        MyClass c(2);
        auto it = myList.begin();
        ++it;
        myList.insert(it, &c);

        int expected[] = {1, 2, 3};
        int i = 0;
        for (auto it = myList.begin(); it != myList.end(); ++it, ++i) {
            REQUIRE(it->value == expected[i]);
        }
    }

    SECTION("erase") {
        MyClass a(1), b(2), c(3);
        myList.push_back(&a);
        myList.push_back(&b);
        myList.push_back(&c);

        auto it = myList.begin();
        ++it;
        myList.erase(it);

        int expected[] = {1, 3};
        int i = 0;
        for (auto it = myList.begin(); it != myList.end(); ++it, ++i) {
            REQUIRE(it->value == expected[i]);
        }
    }

    SECTION("moveList") {
        MemberList<MyClass, &MyClass::listNode> anotherList;
        MyClass a(1), b(2), c(3);
        myList.push_back(&a);
        myList.push_back(&b);

        MyClass d(4), e(5);
        anotherList.push_back(&d);
        anotherList.push_back(&e);

        auto it = myList.begin();
        ++it;
        myList.moveList(it, anotherList);

        int expected[] = {1, 4, 5, 2};
        int i = 0;
        for (auto it = myList.begin(); it != myList.end(); ++it, ++i) {
            REQUIRE(it->value == expected[i]);
        }
    }
}
