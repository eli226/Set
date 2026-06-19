#include <iostream>
#include "../../include/Containers/Set.h"
#include "MM/Mem.h"
#include "../tests/SetTests.h"
using std::cout, std::endl;

int main() {
    Mem mem(1024* 1024* 1024);
    SetTests tests(mem);
    tests.test_insert();
    tests.test_find();

    return 0;
}