#include <iostream>
#include "Containers/Set.h"
#include "MM/Mem.h"
#include "../tests/SetTests.h"
using std::cout, std::endl;

int main() {
    Mem mem(1024* 1024* 1024);
    SetTests tests(mem);
    tests.test_insert();
    tests.test_find();
    tests.test_remove();
    tests.test_empty();
    tests.test_iterator();

    return 0;
}