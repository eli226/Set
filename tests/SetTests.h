#ifndef SET_SETTESTS_H
#define SET_SETTESTS_H
#include "Containers/Set.h"
#include "MM/Mem.h"

class SetTests {
private:
    Set* set;
    Mem* mem;
public:
    SetTests(MemoryManager& mem) {
        set = new Set(mem);
    };
    ~SetTests() {
        delete set;
    }
    void test_insert();
    void test_find();
    void test_remove();
    void test_empty();
    void test_iterator();
};

#endif //SET_SETTESTS_H