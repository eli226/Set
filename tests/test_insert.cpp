#include "SetTests.h"

static bool test_insert_small_sequence(Set* set) {
    set->clear();
    for (size_t i = 0; i < 100; i++) {
        int res = set->insert(&i, sizeof(int));
        if (res != 0) return false;
    }
    return true;
}
static bool test_insert_million_sequence(Set* set) {
    set->clear();
    for (size_t i = 0; i < 1000000; i++) {
        int res = set->insert(&i, sizeof(int));
        if (res != 0) return false;
    }
    return true;
}
static bool test_insert_rand_words(Set* set) {
    set->clear();
    srand(time(nullptr));
    for (size_t i = 0; i < 10; i++) {
        size_t len = rand() % 6 + 3;
        char buff[9];
        for (size_t j = 0; j < len; j++) {
            buff[j] = 'a' + rand() % 26;
        }
        buff[len] = '\0';
        if (set->insert(buff, len + 1) != 0) return false;
    }
    return true;
}
static bool test_insert_duplicate(Set* set) {
    set->clear();
    int a = 1;
    set->insert(&a,sizeof(int));
    if (set->insert(&a, sizeof(int)) != -1) return false;
    return true;
}

void SetTests::test_insert() {
    int count = 0;
    if (test_insert_small_sequence(set) == true) {
        cout << "test 1 passed" << endl;
        count++;
    }
    else
        cout << "test 1 failed" << endl;

    if (test_insert_million_sequence(set) == true) {
        cout << "test 2 passed" << endl;
        count++;
    }
    else
        cout << "test 2 failed" << endl;

    if (test_insert_rand_words(set) == true) {
        cout << "test 3 passed" << endl;
        count++;
    }
    else
        cout << "test 3 failed" << endl;

    if (test_insert_duplicate(set) == true) {
        cout << "test 4 passed" << endl;
        count++;
    }
    else
        cout << "test 4 failed" << endl;

    if (count == 4) {
        cout << "the insert is working properly" << endl;
    }
    else {
        cout << "something went wrong" << endl;
    }
    cout << "\n";
}