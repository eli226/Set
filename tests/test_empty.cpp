#include "SetTests.h"

bool test_new_set_empty(Set* set) {
    set->clear();
    return set->empty() == true;
}
bool test_empty_after_insert(Set* set) {
    set->clear();
    int x = 10;
    set->insert(&x, sizeof(int));

    return set->empty() == false;
}
bool test_empty_after_remove(Set* set) {
    set->clear();
    int x = 10;
    set->insert(&x, sizeof(int));
    AbstractSet::Iterator* it = set->find(&x, sizeof(int));
    if (it == nullptr) return false;
    set->remove(it);
    delete it;

    return set->empty() == true;
}
bool test_empty_after_remove_not_full_sequence(Set* set) {
    set->clear();
    int values[8] = {0,1,2,3,4,5,6,7};
    for (size_t i = 0; i < 8; i++) set->insert(&values[i], sizeof(int));
    for (size_t i = 0; i < 4; i++) {
        AbstractSet::Iterator* it = set->find(&values[i], sizeof(int));
        if (it == nullptr) return false;
        set->remove(it);
        delete it;
    }
    return set->empty() == false;
}

void SetTests::test_empty() {
    int count = 0;
    if (test_new_set_empty(set) == true) {
        cout << "test 1 passed" << endl;
        count++;
    }
    else
        cout << "test 1 failed" << endl;

    if (test_empty_after_insert(set) == true) {
        cout << "test 2 passed" << endl;
        count++;
    }
    else
        cout << "test 2 failed" << endl;

    if (test_empty_after_remove(set) == true) {
        cout << "test 3 passed" << endl;
        count++;
    }
    else
        cout << "test 3 failed" << endl;

    if (test_empty_after_remove_not_full_sequence(set) == true) {
        cout << "test 4 passed" << endl;
        count++;
    }
    else
        cout << "test 4 failed" << endl;

    if (count == 4) {
        cout << "the empty is working properly" << endl;
    }
    else {
        cout << "something went wrong" << endl;
    }
    cout << "\n";
}