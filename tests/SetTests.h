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
    void test_insert(size_t parametr);
    void test_find();
    void test_remove();
    void test_empty();
    void test_iterator(size_t size);
};

#endif //SET_SETTESTS_H