#include <iostream>
#include "../../include/Containers/Set.h"
#include "MM/Mem.h"
using std::cout, std::endl;

int main() {
    Mem memory(1024 * 1024 );
    Set set(memory);
    for (int i = 0; i < 10; i++) {
        set.insert(&i, sizeof(int));
    }
    int i = 0;
    for (AbstractSet::Iterator* it = set.newIterator(); it != nullptr; i++) {
        size_t size;
        void* element = it->getElement(size);
        if (!(*(int*)element == i)) {
            cout << "no equal" << endl;
        }
        else {
            cout << *(int*)element << endl;
        }
        if (!it->hasNext()) {
            delete it;
            break;
        }
        it->goToNext();
    }

    cout << "\n"<< endl;

    AbstractSet::Iterator* it = set.newIterator();
    set.remove(it);
    for (AbstractSet::Iterator* it = set.newIterator(); it != nullptr; i++) {
        size_t size;
        void* element = it->getElement(size);
        if (!(*(int*)element == i)) {
            cout << *(int*)element << endl;
        }
        if (!it->hasNext()) {
            delete it;
            break;
        }
        it->goToNext();
    }

    cout << "\n"<< endl;

    int el = 11;
    AbstractSet::Iterator* it2 = set.find(&el, sizeof(int));
    if (it2 == nullptr) {
        cout << "not found" << endl;
    }
    else {
        size_t size;
        cout << *(int*)it2->getElement(size) << endl;
        delete it2;
    }
    return 0;
}