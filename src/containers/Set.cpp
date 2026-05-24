#include "../../include/Containers/Set.h"
#include "MM/Mem.h"

void *Set::Iterator::getElement(size_t &size) {
    if (_iterator == nullptr) {
        return nullptr;
    }
    return _iterator->getElement(size);
}
bool Set::Iterator::hasNext() {
    if (_iterator == nullptr) {
        return false;
    }
    if (_iterator->hasNext()) return true;
    List2** next = _curr_buckets + 1;
    while (next != _end) {
        if (!(*next)->empty()) {
            return true;
        }
        next++;
    }
    return false;
}

void Set::Iterator::goToNext() {
    bool res = _iterator->hasNext();
    if (res) {
        _iterator->goToNext();
    }
    else {
        _curr_buckets++;
        while (_curr_buckets != _end) {
            if (!(*_curr_buckets)->empty()) {
                delete _iterator;
                _iterator =  dynamic_cast<List2::ListIterator*>((*_curr_buckets)->newIterator());
                break;
            }
            _curr_buckets++;
        }
        if (_curr_buckets == _end) {
            delete _iterator;
            _iterator = nullptr;
        }
    }
}
bool Set::Iterator::equals(Container::Iterator* right) {
    return false;
}
AbstractSet::Iterator* Set::find(void* elem, size_t size) {
    size_t hash = _table->get_hash(elem, size);
    List2::ListIterator* res = dynamic_cast<List2::ListIterator*>(_table->buckets[hash]->find(elem, size));
    if (res == nullptr) {
        return nullptr;
    }
    else {
        return new Iterator(&(_table->buckets[hash]),(_table->buckets + _table->capacity), res);
    }
}
AbstractSet::Iterator* Set::newIterator() { // работает исправно
    List2** _start = _table->buckets;
    List2** _end = _start + _table->capacity;
    List2** curr = _start;
    while (curr != _end) {
        if (!(*curr)->empty()) {
            List2::ListIterator* res = (List2::ListIterator*)((*curr)->newIterator());
            return new Iterator(curr, _end, res);
        }
        curr++;
    }
    return nullptr;
}
int Set::insert(void* elem, size_t size) {
    AbstractSet::Iterator* it = find(elem, size);
    if (it != nullptr) {
        delete it;
        return 1;
    }
    else {
        _table->insert(elem, size);
    }
    return 0;
}
void Set::remove(AbstractSet::Iterator *iter) { //работает исправно
    if (iter == nullptr) { return; }
    Set::Iterator* it = (Set::Iterator*)(iter);
    (*it->getCurrBucket())->remove(it->getCurrBucketIterator());

    if (it->getCurrBucketIterator() == nullptr) {
        it->goToNext();
    }
    if ((*it->getCurrBucket())->empty()) {
        //_table->_used.remove(*it->getCurrBucket());
    }
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