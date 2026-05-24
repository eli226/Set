#pragma once
#include "AbstractClasses/SetAbstract.h"
#include "List2.h"
#include "Table.h"
#include "UsedBuck.h"

class Set final : public AbstractSet  {
private:
    Table* _table;
public:
    class Iterator : public AbstractSet::Iterator {
    private:
        List2::ListIterator* _iterator;
        List2** _curr_buckets;
        List2** _end;

    public:
        Iterator(List2** curr_buckets, List2** end, List2::ListIterator* iterator) :
        _iterator(iterator), _curr_buckets(curr_buckets), _end(end) {

        };
        ~Iterator() override {
            delete _iterator;
        }
        List2** getCurrBucket() {
            return _curr_buckets;
        }
        List2::ListIterator* getCurrBucketIterator() {
            return _iterator;
        }
        void* getElement(size_t &size) override;
        bool hasNext() override;
        void goToNext() override;
        bool equals(Container::Iterator *right) override;
    };
    Set(MemoryManager& mem) : AbstractSet(mem) {
        _table = new Table(mem);
    };
    ~Set() {
        delete _table;
    }

    int insert(void *elem, size_t size) override;
    int size() override;
    void remove(AbstractSet::Iterator* iter) override;
    bool empty() override;
    void clear() override;
    size_t max_bytes() override;
    AbstractSet::Iterator* find(void *elem, size_t size) override;
    AbstractSet::Iterator* newIterator() override;
};