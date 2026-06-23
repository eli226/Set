#include <iostream>
#include "Containers/Set.h"
#include "MM/Mem.h"
#include "../tests/SetTests.h"
using std::cout, std::endl;

int main() {
    Mem mem(1024* 1024* 1024);
    SetTests tests(mem);
    //tests.test_insert(1000000);
    tests.test_iterator(100000);
    return 0;
}