#include <iostream>
#include "../../include/Containers/Set.h"
#include "MM/Mem.h"
#include <time.h>
using std::cout, std::endl;

int main() {
    Mem memory(10000* 10000);
    Set set(memory);
    const char* str1 = "hello";
    const char* str2 = "world";
    const char* str3 = "hello world";
    const char* str4 = "bla bla bla";
    const char* str5 = "1234";
    set.insert((void*)str1, strlen(str1) + 1);
    set.insert((void*)str2, strlen(str2) + 1);
    set.insert((void*)str3, strlen(str3) + 1);
    set.insert((void*)str4, strlen(str4) + 1);
    set.insert((void*)str5, strlen(str5) + 1);
    cout << "\n" <<  endl;
    for (AbstractSet::Iterator* it = set.newIterator(); it != nullptr;) {
        size_t size;
        void* element = it->getElement(size);
        cout << (char*)element << endl;

        if (!it->hasNext()) {
            delete it;
            break;
        }
        it->goToNext();
    }
    for (size_t i = 0; i < 100000; i++) {
        set.insert(&i, sizeof(int));
    }
    return 0;
}