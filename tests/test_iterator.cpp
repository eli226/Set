#include "SetTests.h"
#include <chrono>
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
bool test_get_element(Set*set, size_t len) {
    set->clear();

    auto start = std::chrono::high_resolution_clock::now();
    for (int i = 0; i < len; i++) {
        int res = set->insert(&i, sizeof(int));
        if (res == -1) return false;
    }

    AbstractSet::Iterator* it = set->newIterator();
    if (!it) return false;

    size_t size;
    size_t count = 0;
    bool* find = new bool[len]();

    while (true) {
        void* res = it->getElement(size);

        if (res == nullptr) {
            delete[] find;
            delete it;
            return false;
        }

        int value = *static_cast<int*>(res);

        if (value < 0 || value >= len) {
            delete[] find;
            delete it;
            return false;
        }

        if (find[value]) {
            delete[] find;
            delete it;
            return false;
        }

        find[value] = true;
        count++;

        if (it->hasNext())
            it->goToNext();
        else
            break;
    }

    delete it;

    if (count != len) {
        delete[] find;
        return false;
    }

    for (int i = 0; i < len / 2; i++) {
        AbstractSet::Iterator* rem = set->find(&i, sizeof(int));

        if (rem == nullptr) {
            delete[] find;
            return false;
        }

        set->remove(rem);
        delete rem;
    }

    bool* find2 = new bool[len]();

    AbstractSet::Iterator* it2 = set->newIterator();
    if (!it2) {
        delete[] find;
        delete[] find2;
        return false;
    }

    size_t count2 = 0;

    while (true) {
        void* res = it2->getElement(size);

        if (res == nullptr) {
            delete[] find;
            delete[] find2;
            delete it2;
            return false;
        }

        int value = *static_cast<int*>(res);

        if (value < len / 2 || value >= len) {
            delete[] find;
            delete[] find2;
            delete it2;
            return false;
        }

        if (find2[value]) {
            delete[] find;
            delete[] find2;
            delete it2;
            return false;
        }

        find2[value] = true;
        count2++;

        if (it2->hasNext())
            it2->goToNext();
        else
            break;
    }

    delete it2;

    auto end = std::chrono::high_resolution_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(end - start);

    std::cout << duration.count() / 1000.0 << "s" << endl;

    if (count2 != len - len / 2) {
        delete[] find;
        delete[] find2;
        return false;
    }

    for (size_t i = len / 2; i < len; i++) {
        if (!find2[i]) {
            delete[] find;
            delete[] find2;
            return false;
        }
    }

    delete[] find;
    delete[] find2;

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
// bool check(Set* set, size_t len) {
//     set->clear();
//     for (int i = 0; i < len; i++) set->insert(&i, sizeof(int));
//
//     AbstractSet::Iterator* it = set->newIterator();
//     while (it) {
//         int flag = false;
//         size_t size;
//         void* res = it->getElement(size);
//         if ()
//     }
// }
void SetTests::test_iterator(size_t size) {
    int count = 0;
    // if (test_new_iterator(set) == true) {
    //     cout << "test 1 passed" << endl;
    //     count++;
    // }
    // else
    //     cout << "test 1 failed" << endl;
    //
    // if (test_passing_through_all_elements(set) == true) {
    //     cout << "test 2 passed" << endl;
    //     count++;
    // }
    // else
    //     cout << "test 2 failed" << endl;

    if (test_get_element(set, size) == true) {
        cout << "test 3 passed" << endl;
        count++;
    }
    else
        cout << "test 3 failed" << endl;

    // if (test_has_next(set) == true) {
    //     cout << "test 4 passed" << endl;
    //     count++;
    // }
    // else
    //     cout << "test 4 failed" << endl;
    //
    // if (test_equals_iterators(set) == true) {
    //     cout << "test 5 passed" << endl;
    //     count++;
    // }
    // else
    //     cout << "test 5 failed" << endl;
    //
    // if (test_no_equals_iterators(set) == true) {
    //     cout << "test 6 passed" << endl;
    //     count++;
    // }
    // else
    //     cout << "test 6 failed" << endl;
    //
    // if (count == 6) {
    //     cout << "the iterator is working properly" << endl;
    // }
    // cout << "\n";
}