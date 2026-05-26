#include <iostream>
#include "../../include/Containers/Set.h"
#include "MM/Mem.h"
using std::cout, std::endl;

int passed = 0;
int failed = 0;

void check(const char* name, bool condition) {
    if (condition) {
        cout << "[OK]   " << name << endl;
        passed++;
    } else {
        cout << "[FAIL] " << name << endl;
        failed++;
    }
}

void test_insert() {
    cout << "\n=== insert ===" << endl;
    Mem mem(1024 * 1024);
    Set set(mem);

    int a = 10, b = 20;
    const char* s = "hello";

    // 1. Успешная вставка int
    check("insert int returns 0", set.insert(&a, sizeof(int)) == 0);

    // 2. Дубликат — возвращает 1
    check("insert duplicate returns 1", set.insert(&a, sizeof(int)) == 1);

    // 3. Успешная вставка другого int
    check("insert another int returns 0", set.insert(&b, sizeof(int)) == 0);

    // 4. Успешная вставка строки
    check("insert string returns 0", set.insert((void*)s, strlen(s) + 1) == 0);

    // 5. Дубликат строки
    check("insert duplicate string returns 1", set.insert((void*)s, strlen(s) + 1) == 1);

    // 6. Вставка после дубликата не ломает состояние
    int c = 30;
    check("insert after duplicate ok", set.insert(&c, sizeof(int)) == 0);
}

void test_find() {
    cout << "\n=== find ===" << endl;
    Mem mem(1024 * 1024);
    Set set(mem);

    int a = 42, b = 99;
    set.insert(&a, sizeof(int));

    // 1. Найти существующий элемент
    AbstractSet::Iterator* it = set.find(&a, sizeof(int));
    check("find existing returns non-null", it != nullptr);
    delete it;

    // 2. Найти несуществующий элемент
    it = set.find(&b, sizeof(int));
    check("find missing returns null", it == nullptr);

    // 3. Найти и проверить значение
    it = set.find(&a, sizeof(int));
    size_t sz;
    void* elem = it->getElement(sz);
    check("find returns correct value", *(int*)elem == 42);
    delete it;

    // 4. find на пустом множестве
    Set empty_set(mem);
    it = empty_set.find(&a, sizeof(int));
    check("find on empty set returns null", it == nullptr);

    // 5. find строки
    const char* s = "world";
    set.insert((void*)s, strlen(s) + 1);
    it = set.find((void*)s, strlen(s) + 1);
    check("find string returns non-null", it != nullptr);
    delete it;

    // 6. find после удаления
    AbstractSet::Iterator* it2 = set.find(&a, sizeof(int));
    set.remove(it2);
    it = set.find(&a, sizeof(int));
    check("find after remove returns null", it == nullptr);
}

void test_remove() {
    cout << "\n=== remove ===" << endl;
    Mem mem(1024 * 1024);
    Set set(mem);

    int a = 1, b = 2, c = 3;
    set.insert(&a, sizeof(int));
    set.insert(&b, sizeof(int));
    set.insert(&c, sizeof(int));

    // 1. remove nullptr не падает
    set.remove(nullptr);
    check("remove nullptr does not crash", true);

    // 2. После remove элемент не находится
    AbstractSet::Iterator* it = set.find(&a, sizeof(int));
    set.remove(it);
    it = set.find(&a, sizeof(int));
    check("removed element not found", it == nullptr);

    // 3. Остальные элементы на месте
    it = set.find(&b, sizeof(int));
    check("other elements intact after remove", it != nullptr);
    delete it;

    // 4. Удалить все элементы по одному
    it = set.find(&b, sizeof(int));
    set.remove(it);
    it = set.find(&c, sizeof(int));
    set.remove(it);
    check("set empty after removing all", set.empty());

    // 5. Вставка после удаления работает
    check("insert after remove ok", set.insert(&a, sizeof(int)) == 0);
}

void test_empty() {
    cout << "\n=== empty ===" << endl;
    Mem mem(1024 * 1024);
    Set set(mem);

    // 1. Пустое множество
    check("new set is empty", set.empty());

    // 2. После вставки не пустое
    int a = 5;
    set.insert(&a, sizeof(int));
    check("non-empty after insert", !set.empty());

    // 3. После удаления снова пустое
    AbstractSet::Iterator* it = set.find(&a, sizeof(int));
    set.remove(it);
    check("empty after remove", set.empty());

    // 4. Несколько вставок — не пустое
    int b = 10, c = 20;
    set.insert(&a, sizeof(int));
    set.insert(&b, sizeof(int));
    set.insert(&c, sizeof(int));
    check("not empty with multiple elements", !set.empty());
}

void test_iterator() {
    cout << "\n=== iterator ===" << endl;
    Mem mem(1024 * 1024);
    Set set(mem);

    // 1. newIterator на пустом — nullptr
    check("newIterator on empty returns null", set.newIterator() == nullptr);

    // 2. Итерация проходит все элементы
    int vals[5] = {1, 2, 3, 4, 5};
    for (int i = 0; i < 5; i++)
        set.insert(&vals[i], sizeof(int));

    int count = 0;
    for (AbstractSet::Iterator* it = set.newIterator(); it != nullptr; ) {
        count++;
        if (!it->hasNext()) { delete it; break; }
        it->goToNext();
    }
    check("iterator visits all 5 elements", count == 5);

    // 3. getElement не возвращает nullptr
    AbstractSet::Iterator* it = set.newIterator();
    size_t sz;
    check("getElement returns non-null", it->getElement(sz) != nullptr);
    delete it;

    // 4. equals — два итератора на один элемент
    int x = 42;
    set.insert(&x, sizeof(int));
    AbstractSet::Iterator* it1 = set.find(&x, sizeof(int));
    AbstractSet::Iterator* it2 = set.find(&x, sizeof(int));
    check("equals same element returns true", it1->equals(it2));
    delete it1;
    delete it2;

    // 5. equals — разные элементы
    int y = 1;
    it1 = set.find(&x, sizeof(int));
    it2 = set.find(&y, sizeof(int));
    check("equals different elements returns false", !it1->equals(it2));
    delete it1;
    delete it2;
}

int main() {
    test_insert();
    test_find();
    test_remove();
    test_empty();
    test_iterator();

    cout << "\n=== Results: " << passed << " passed, " << failed << " failed ===" << endl;
    return failed == 0 ? 0 : 1;
}