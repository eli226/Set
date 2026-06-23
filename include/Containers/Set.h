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
        Container::Iterator* _iter;
    public:
        Iterator(Container::Iterator* iter): _iter(iter) {};
        ~Iterator() override {
            delete _iter;
        }
        Container::Iterator* getTableIterator() { return _iter; }
        void* getElement(size_t &size) override;
        bool hasNext() override;
        void goToNext() override;
        bool equals(Container::Iterator *right) override;
    };
    Set(MemoryManager& mem) : AbstractSet(mem) {
        _table = new Table(mem);
    };
    ~Set() override {
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