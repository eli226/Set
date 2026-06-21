#include "SetTests.h"

bool test_new_iterator(Set* set) {
    set->clear();
    AbstractSet::Iterator* it = set->newIterator();
    if (it !=  nullptr) return false;

    return true;
}
bool test_passing_through_all_elements(Set* set) {
    set->clear();
    int count = 0;
    int real_size;

    for (size_t i = 0; i < 10; i++) set->insert(&i, sizeof(int));
    AbstractSet::Iterator* it = set->newIterator();

    real_size = set->size();

    while (it != nullptr) {
        count++;
        if (it->hasNext()) {
            it->goToNext();
        }
        else break;
    }

    delete it;
    return count == real_size;
}
bool test_get_element(Set*set) {
    set->clear();
    int values[10] = {0,1,2,3,4,5,6,7,8,9,};
    for (size_t i = 0; i < 10; i++) set->insert(&values[i], sizeof(int));

    AbstractSet::Iterator* it = set->newIterator();
    size_t size;

    while (it != nullptr) {
        bool flag = false;
        void* res = it->getElement(size);
        for (size_t i = 0; i < 10; i++) {
            if (*static_cast<int*>(res) == values[i]) {
                flag = true;
                break;
            }
        }
        if (flag == false) return false;
        if (it->hasNext() != false) it->goToNext();
        else break;
    }
    delete it;
    return true;
}
bool test_has_next(Set* set) {
    set->clear();
    int x = 10;
    set->insert(&x, sizeof(int));

    AbstractSet::Iterator* it = set->newIterator();
    if (it == nullptr) return false;
    bool res = it->hasNext();

    delete it;
    return res == false;
}
bool test_equals_iterators(Set* set) {
    set->clear();
    int values[10] = {0,1,2,3,4,5,6,7,8,9,};
    for (size_t i = 0; i < 10; i++) set->insert(&values[i], sizeof(int));

    AbstractSet::Iterator* it1 = set->newIterator();
    AbstractSet::Iterator* it2 = set->newIterator();

    bool res = it1->equals(it2);

    delete it1;
    delete it2;
    return res == true;
}
bool test_no_equals_iterators(Set* set) {
    set->clear();
    int x = 10, y = 5;
    set->insert(&x, sizeof(int));
    set->insert(&y, sizeof(int));

    AbstractSet::Iterator* it1 = set->find(&x, sizeof(int));
    AbstractSet::Iterator* it2 = set->find(&y, sizeof(int));

    bool res = it1->equals(it2);
    delete it1;
    delete it2;

    return res == false;
}
void SetTests::test_iterator() {
    int count = 0;
    if (test_new_iterator(set) == true) {
        cout << "test 1 passed" << endl;
        count++;
    }
    else
        cout << "test 1 failed" << endl;

    if (test_passing_through_all_elements(set) == true) {
        cout << "test 2 passed" << endl;
        count++;
    }
    else
        cout << "test 2 failed" << endl;

    if (test_get_element(set) == true) {
        cout << "test 3 passed" << endl;
        count++;
    }
    else
        cout << "test 3 failed" << endl;

    if (test_has_next(set) == true) {
        cout << "test 4 passed" << endl;
        count++;
    }
    else
        cout << "test 4 failed" << endl;

    if (test_equals_iterators(set) == true) {
        cout << "test 5 passed" << endl;
        count++;
    }
    else
        cout << "test 5 failed" << endl;

    if (test_no_equals_iterators(set) == true) {
        cout << "test 6 passed" << endl;
        count++;
    }
    else
        cout << "test 6 failed" << endl;

    if (count == 6) {
        cout << "the iterator is working properly" << endl;
    }
    cout << "\n";
}