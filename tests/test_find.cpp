#include "SetTests.h"

static bool test_find_elem(Set* set) {
    set->clear();
    int x = 5, y = 10;
    set->insert(&x, sizeof(int));
    set->insert(&y, sizeof(int));

    AbstractSet::Iterator *res = set->find(&x, sizeof(int));
    if (res == nullptr) return false;
    delete res;

    return true;
}
static bool test_find_words(Set* set) {
    set->clear();
    char buff[6] = {'h', 'e', 'l', 'l', 'o', '\0'};
    size_t len = 6;
    set->insert(&buff, len);
    AbstractSet::Iterator* res = set->find(&buff, len);
    if (res == nullptr) return false;
    delete res;

    return true;
}
static bool test_find_non_existent_elem(Set* set) {
    set->clear();
    int x = 10;

    AbstractSet::Iterator* res = set->find(&x, sizeof(int));
    if (res != nullptr) {
        delete res;
        return false;
    }

    return true;
}
static bool test_find_after_remove(Set* set) {
    set->clear();
    int x = 10;
    set->insert(&x, sizeof(int));

    AbstractSet::Iterator* it = set->find(&x, sizeof(int));
    set->remove(it);
    delete it;

    AbstractSet::Iterator* res = set->find(&x, sizeof(int));
    if (res != nullptr) return false;

    delete res;
    return true;
}
static bool test_find_another_type(Set* set) {
    set->clear();
    size_t size1 = 0, size2 = 0;
    double x = 10;
    int y = 10;
    set->insert(&x, sizeof(double));
    set->insert(&y, sizeof(int));

    AbstractSet::Iterator* it1 = set->find(&x, sizeof(double));
    AbstractSet::Iterator* it2 = set->find(&y, sizeof(int));

    void* val1 = it1->getElement(size1);
    void* val2 = it2->getElement(size2);
    if (size1 == size2) return false;

    delete it1;
    delete it2;

    return true;
}

void SetTests::test_find() {
    int count = 0;
    if (test_find_elem(set) == true) {
        cout << "test 1 passed" << endl;
        count++;
    }
    else
        cout << "test 1 failed" << endl;

    if (test_find_non_existent_elem(set) == true) {
        cout << "test 2 passed" << endl;
        count++;
    }
    else
        cout << "test 2 failed" << endl;

    if (test_find_after_remove(set) == true) {
        cout << "test 3 passed" << endl;
        count++;
    }
    else
        cout << "test 3 failed" << endl;

    if (test_find_words(set) == true) {
        cout << "test 4 passed" << endl;
        count++;
    }
    else
        cout << "test 4 failed" << endl;

    if (test_find_another_type(set) == true) {
        cout << "test 5 passed" << endl;
        count++;
    }
    else
        cout << "test 5 failed" << endl;

    if (count == 5) {
        cout << "the find is working properly" << endl;
    }
    cout << "\n" << endl;
}