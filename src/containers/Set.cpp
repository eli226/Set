#include "../../include/Containers/Set.h"
#include "MM/Mem.h"

void *Set::Iterator::getElement(size_t &size) {
    if (_iterator == nullptr) {
        return nullptr;
    }
    return _iterator->getElement(size);
}
bool Set::Iterator::hasNext() {// подумать над ускорением, лучшая идея которая у меня появилась это список
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
    bool res;
    res = hasNext();
    if (res == false) {
        _curr_buckets++;
        while (_curr_buckets != _end) {
            if (!(*_curr_buckets)->empty()) {
                delete _iterator;
                _iterator = dynamic_cast<List2::ListIterator*>((*_curr_buckets)->newIterator());
                break;
            }
            _curr_buckets++;
        }
        if (_curr_buckets == _end) {
            _iterator = nullptr;
        }
    }
    else {
        if (_iterator->hasNext()) {
            _iterator->goToNext();
        } else {
            _curr_buckets++;
            while (_curr_buckets != _end) {
                if (!(*_curr_buckets)->empty()) {
                    delete _iterator;
                    _iterator = dynamic_cast<List2::ListIterator*>((*_curr_buckets)->newIterator());
                    break;
                }
                _curr_buckets++;
            }
            if (_curr_buckets == _end) {
                _iterator = nullptr;
            }
        }
    }
}
bool Set::Iterator::equals(Container::Iterator* right) {
    return false;
}
AbstractSet::Iterator* Set::find(void* elem, size_t size) {
    List2** _start = _table->buckets;
    List2** _end = _start + _table->capacity;
    List2** curr = _start;
    List2::ListIterator* res;
    while (curr != _end) {
        if (!(*curr)->empty()) {
            res = (List2::ListIterator*)((*curr)->find(elem, size));
            if (res != nullptr) {
                return new Iterator(curr, _end, res);
            }
        }
        curr++;
    }
    return nullptr;
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
    if (find(elem, size) != nullptr)
        return -1;
    else {
        size_t hash = _table->get_hash(elem, size);
        _table->buckets[hash]->push_front(elem, size);
        return 0;
    }
    return -2;
}
void Set::remove(AbstractSet::Iterator *iter) { //работает исправно
    if (iter == nullptr) { return; }

    Set::Iterator* it = (Set::Iterator*)(iter);
    if (it == nullptr) { return; }

    (*it->getCurrBucket())->remove(it->getCurrBucketIterator());

    if (it->getCurrBucketIterator() == nullptr) {
        it->goToNext();
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
    List2** start = _table->buckets;
    List2** end = start + _table->capacity;
    List2** curr = start;
    while (curr != end) {
        if (!(*curr)->empty()) {
            return false;
        }
        curr++;
    }
    return true;
}