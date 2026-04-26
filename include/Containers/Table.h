#pragma once
#include "AbstractClasses/TableAbstract.h"
#include "List2.h"
#include <cstring>

class Set;

class Table : public AbstractTable {
    friend class Set;
private:
    List2** buckets;
    size_t capacity;
    int count;

    size_t get_hash(void* data, size_t size) {
        size_t hash_value = 0;
        unsigned char* bytes = (unsigned char*)data;
        for (size_t i = 0; i < size; i++)
            hash_value += bytes[i];
        return hash_value % capacity;
    }

public:
    Table(MemoryManager& mem) : AbstractTable(mem), capacity(101), count(0) {
        buckets = new List2*[capacity];
        for (size_t i = 0; i < capacity; i++)
            buckets[i] = new List2(mem);
    }

    ~Table() {
        for (size_t i = 0; i < capacity; i++)
            delete buckets[i];
        delete[] buckets;
    }

    // Вставка элемента
    void insert(void* elem, size_t size) {
        size_t idx = get_hash(elem, size);
        buckets[idx]->push_front(elem, size);
        count++;
    }

    // Удаление элемента по значению
    void removeElem(void* elem, size_t size) {
        size_t idx = get_hash(elem, size);
        Iterator* it = buckets[idx]->find(elem, size);
        if (it != nullptr) {
            buckets[idx]->remove(it);
            delete it;
            count--;
        }
    }

    // Container
    int size() override { return count; }
    size_t max_bytes() override { return _memory.size(); }
    bool empty() override { return count == 0; }

    void clear() override {
        for (size_t i = 0; i < capacity; i++)
            buckets[i]->clear();
        count = 0;
    }

    Iterator* find(void* elem, size_t size) override {
        size_t idx = get_hash(elem, size);
        return buckets[idx]->find(elem, size);
    }

    Iterator* newIterator() override {
        for (size_t i = 0; i < capacity; i++) {
            if (!buckets[i]->empty())
                return buckets[i]->newIterator();
        }
        return nullptr;
    }

    void remove(Iterator* iter) override {}

    // Заглушки AbstractTable
    int insertByKey(void* key, size_t keySize, void* elem, size_t elemSize) override { return 1; }
    void removeByKey(void* key, size_t keySize) override {}
    Iterator* findByKey(void* key, size_t keySize) override { return nullptr; }
    void* at(void* key, size_t keySize, size_t& valueSize) override { return nullptr; }
};