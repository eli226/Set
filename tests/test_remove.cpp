#include "SetTests.h"

static bool test_remove_elem(Set* set) {
    set->clear();
    int x = 10;
    set->insert(&x, sizeof(int));
    AbstractSet::Iterator* it = set->find(&x, sizeof(int));
    if (it == nullptr) return false;

    set->remove(it);
    delete it;

    AbstractSet::Iterator* res = set->find(&x, sizeof(int));
    if (res != nullptr) {
        delete res;
        return false;
    }
    return true;
}
static bool test_remove_and_check_size(Set* set) {
    set->clear();
    int x = 10;
    set->insert(&x, sizeof(int));
    int size_bef = static_cast<int>(set->size());
    if (size_bef == 0) return false;

    AbstractSet::Iterator* it = set->find(&x, sizeof(int));
    if (it == nullptr) return false;
    set->remove(it);
    delete it;

    int size_af = static_cast<int>(set->size());
    if (size_af != (size_bef - 1)) return false;

    return true;
}
static bool test_remove_sequence(Set* set) {
    set->clear();
    int values[10] = {0,1,2,3,4,5,6,7,8,9};
    for (size_t i = 0; i < 10; i++) set->insert(&values[i], sizeof(int));

    for (size_t i = 0; i < 5; i++) {
        AbstractSet::Iterator* it = set->find(&values[i], sizeof(int));
        set->remove(it);
        delete it;
    }
    for (size_t i = 0; i < 5; i++) {
        AbstractSet::Iterator* it = set->find(&values[i], sizeof(int));
        if (it != nullptr) {
            delete it;
            return false;
        }
    }
    for (size_t i = 5; i < 10; i++) {
        AbstractSet::Iterator* it = set->find(&values[i], sizeof(int));
        if (it == nullptr) return false;
        delete it;
    }
    return true;
}
static bool test_remove_nullptr(Set* set) {
    set->clear();
    set->remove(nullptr);
    return true;
}
static bool test_insert_after_remove(Set* set) {
    set->clear();
    int x = 10;
    set->insert(&x, sizeof(int));
    AbstractSet::Iterator* it1 = set->find(&x, sizeof(int));
    if (it1 == nullptr) return false;
    set->remove(it1);
    delete it1;

    set->insert(&x, sizeof(int));
    AbstractSet::Iterator* it2 = set->find(&x, sizeof(int));
    if (it2 == nullptr) return false;
    delete it2;

    return true;
}
void SetTests::test_remove() {
    int count = 0;
    if (test_remove_elem(set) == true) {
        cout << "test 1 passed" << endl;
        count++;
    }
    else
        cout << "test 1 failed" << endl;

    if (test_remove_and_check_size(set) == true) {
        cout << "test 2 passed" << endl;
        count++;
    }
    else
        cout << "test 2 failed" << endl;

    if (test_remove_sequence(set) == true) {
        cout << "test 3 passed" << endl;
        count++;
    }
    else
        cout << "test 3 failed" << endl;

    if (test_remove_nullptr(set) == true) {
        cout << "test 4 passed" << endl;
        count++;
    }
    else
        cout << "test 4 failed" << endl;

    if (test_insert_after_remove(set) == true) {
        cout << "test 5 passed" << endl;
        count++;
    }
    else
        cout << "test 5 failed" << endl;

    if (count == 5) {
        cout << "the remove is working properly" << endl;
    }
    else {
        cout << "something went wrong" << endl;
    }
    cout << "\n";
}