#include <memberlinkedlist.h>

class Data
{
public:
    Data()
        : index(0)
    {}
    Data(int index)
        : index(index)
    {}
    int index;
    MemberListNode<Data> node;
};


int main()
{
    MemberList<Data, &Data::node> list;
    list.append(new Data(0));
    list.append(new Data(1));
    list.append(new Data(2));

    Data *node = list.first();
    fprintf(stderr, "Printing list\n");
    while(node) {
        fprintf(stderr, "Node index %d\n", node->index);
        node = list.getListNode(node).next();
    }

    MemberList<Data, &Data::node> list2;
    list2.append(new Data(3));
    list2.append(new Data(4));
    list2.append(new Data(5));

    fprintf(stderr, "Printing list2\n");
    for (auto it = list2.begin(); it != list2.end(); ++it) {
        fprintf(stderr, "Node index %d\n", it->index);
    }

    MemberList<Data, &Data::node>::iterator it2 = list.begin();
    it2++;
    auto it3 = list.insert(it2, new Data(6));
    fprintf(stderr, "Printing list again:\n");
    for (auto it4 = list.begin(); it4 != list.end(); ++it4) {
        fprintf(stderr, "Node index %d\n", it4->index);
    }

    list.moveList(it3, list2);
    fprintf(stderr, "Printing combined list:\n");
    for (auto it5 = list.begin(); it5 != list.end(); ++it5) {
        fprintf(stderr, "Node index %d\n", it5->index);
    }

    fprintf(stderr, "And backwards:\n");
    MemberList<Data, &Data::node>::iterator it6;
    for (it6 = --list.end(); it6 != list.begin(); --it6) {
        fprintf(stderr, "Node index %d\n", it6->index);
    }
    fprintf(stderr, "Node index %d\n", it6->index);

    it6++;
    it6++;
    it6++;
    list.erase(it6);
    fprintf(stderr, "Printing list after removing index 3.\n");
    for (auto it7 = list.begin(); it7 != list.end(); ++it7) {
        fprintf(stderr, "Node index %d\n", it7->index);
    }

    return 0;
}
