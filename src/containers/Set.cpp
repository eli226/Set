#include "../../include/Containers/Set.h"
#include "MM/Mem.h"

void *Set::Iterator::getElement(size_t &size) {
    return _iter->getElement(size);
}
bool Set::Iterator::hasNext() {
    return _iter->hasNext();
}
void Set::Iterator::goToNext() {
   return _iter->goToNext();
}
bool Set::Iterator::equals(Container::Iterator* right) {
    Set::Iterator* it = dynamic_cast<Set::Iterator*>(right);
    if (it == nullptr) return false;
    return _iter->equals(it->getTableIterator());
}
AbstractSet::Iterator* Set::find(void* elem, size_t size) {
    Container::Iterator* curr = _table->find(elem, size);
    if (curr != nullptr) {
        return new Set::Iterator(curr);
    }
    else
        return nullptr;
}
AbstractSet::Iterator* Set::newIterator() { // работает исправно
    Container::Iterator* curr = _table->newIterator();
    if (curr != nullptr) {
        return new Set::Iterator(curr);
    }
    else
        return nullptr;
}
int Set::insert(void* elem, size_t size) {
    int res = _table->insertByKey(elem, size, elem, size);
    return (res == 0) ? 0 : -1;
}
void Set::remove(Container::Iterator* iter) {
    if (iter == nullptr) return;
    Set::Iterator* it = dynamic_cast<Set::Iterator*>(iter);
    if (it == nullptr) return;
    _table->remove(it->getTableIterator());
}
int Set::size() { //не тестировал
    return _table->size();
}
size_t Set::max_bytes() { // не тестировал
    return  _table->max_bytes();
}
void Set::clear() { // не тестировал
    _table->clear();
}
bool Set::empty() { // не тестировал
    return _table->empty();
}