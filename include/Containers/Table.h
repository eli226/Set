#pragma once

#include "AbstractClasses/ListAbstract.h"
#include "AbstractClasses/TableAbstract.h"

struct KV
{
    int key;
    int value;
};

struct KV_str
{
    const char* key;
    const char* value;
};

class Table : public AbstractTable
{
public:
    typedef AbstractList* (*BucketFactory)(MemoryManager&);

    Table(MemoryManager& mem, size_t bucketCount = 17, BucketFactory factory = 0);
    ~Table() override;

    int insertByKey(void* key, size_t keySize, void* elem, size_t elemSize) override;
    void removeByKey(void* key, size_t keySize) override;
    Iterator* findByKey(void* key, size_t keySize) override;
    void* at(void* key, size_t keySize, size_t& valueSize) override;

    int size() override;
    size_t max_bytes() override;
    Iterator* find(void* elem, size_t size) override;
    Iterator* newIterator() override;
    void remove(Iterator* iter) override;
    void clear() override;
    bool empty() override;

    // Дополнительные функции работы с конкретными типами данных.
    int insert_by_key(KV* elem, size_t elemSize);
    int insert_string(KV_str* elem, size_t elemSize);
    void print();
    void print_string();

    size_t bucketCount() const { return _bucketCount; }

private:
    class TableIterator;

    static const size_t HEADER_SIZE = sizeof(size_t) * 2;
    static const size_t MAX_LOAD_NUM = 3;
    static const size_t MAX_LOAD_DEN = 4;

    AbstractList** _buckets;
    size_t _bucketCount;
    size_t _size;
    BucketFactory _factory;

    size_t bucketIndex(void* key, size_t keySize) const;
    Iterator* findRawByKey(size_t bucket, void* key, size_t keySize);
    void moveIteratorToNextBucket(TableIterator* it);

    bool needsRehash() const;
    int rehash();
    static size_t hashKey(void* key, size_t keySize);
    static bool isPrime(size_t n);
    static size_t nextPrime(size_t n);
    static size_t nextBucketCount(size_t current);

    // Вспомогательные фукнции.
    static void writeSize(void* ptr, size_t value);
    static size_t readSize(void* ptr);
    char* makeItem(void* key, size_t keySize, void* elem, size_t elemSize);
    static size_t itemKeySize(void* item);
    static size_t itemValueSize(void* item);
    static void* itemKey(void* item);
    static void* itemValue(void* item);
};
