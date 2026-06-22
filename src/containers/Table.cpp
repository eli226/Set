#include "Containers/Table.h"
#include "Containers/List.h"
#include <string.h>
#include <stdio.h>

Container::Iterator::~Iterator() {}

static AbstractList* createDefaultBucket(MemoryManager& mem)
{
    return new List(mem);
}

class Table::TableIterator : public Iterator
{
public:
    TableIterator(Table* t, size_t b, Iterator* i)
    {
        table = t;
        bucket = b;
        inner = i;
    }

    ~TableIterator() override
    {
        delete inner;
    }

    void* getElement(size_t& size) override
    {
        size_t itemSize;
        void* item;

        if (inner == 0)
        {
            size = 0;
            return 0;
        }

        itemSize = 0;
        item = inner->getElement(itemSize);
        if (item == 0)
        {
            size = 0;
            return 0;
        }

        size = itemValueSize(item);
        return itemValue(item);
    }

    bool hasNext() override
    {
        size_t i;

        if (inner != 0 && inner->hasNext())
            return 1;

        for (i = bucket + 1; i < table->_bucketCount; i++)
        {
            if (table->_buckets[i]->empty() == 0)
                return 1;
        }

        return 0;
    }

    void goToNext() override
    {
        if (inner != 0 && inner->hasNext())
        {
            inner->goToNext();
            return;
        }

        table->moveIteratorToNextBucket(this);
    }

    bool equals(Iterator* right) override
    {
        TableIterator* r;

        if (right == 0)
            return 0;

        r = (TableIterator*)right;
        if (table != r->table || bucket != r->bucket)
            return 0;

        if (inner == 0 || r->inner == 0)
            return inner == r->inner;

        return inner->equals(r->inner);
    }

    Table* table;
    size_t bucket;
    Iterator* inner;
};

Table::Table(MemoryManager& mem, size_t bucketCount, BucketFactory factory): AbstractTable(mem)
{
    size_t i;

    _buckets = 0;
    _bucketCount = bucketCount;
    if (_bucketCount == 0)
        _bucketCount = 1;

    _size = 0;
    _factory = factory;
    if (_factory == 0)
        _factory = createDefaultBucket;

    _buckets = (AbstractList**)_memory.allocMem(sizeof(AbstractList*) * _bucketCount);
    for (i = 0; i < _bucketCount; i++)
    {
        _buckets[i] = _factory(_memory);
    }
}

Table::~Table()
{
    size_t i;

    clear();

    for (i = 0; i < _bucketCount; i++)
    {
        delete _buckets[i];
    }

    _memory.freeMem(_buckets);
}

void Table::writeSize(void* ptr, size_t value)
{
    memcpy(ptr, &value, sizeof(size_t));
}

size_t Table::readSize(void* ptr)
{
    size_t value;

    value = 0;
    memcpy(&value, ptr, sizeof(size_t));
    return value;
}

char* Table::makeItem(void* key, size_t keySize, void* elem, size_t elemSize)
{
    char* item;

    item = (char*)_memory.allocMem(HEADER_SIZE + keySize + elemSize);
    if (item == 0)
        return 0;

    writeSize(item, keySize);
    writeSize(item + sizeof(size_t), elemSize);

    if (keySize)
        memcpy(item + HEADER_SIZE, key, keySize);

    if (elemSize)
        memcpy(item + HEADER_SIZE + keySize, elem, elemSize);

    return item;
}

size_t Table::itemKeySize(void* item)
{
    return readSize(item);
}

size_t Table::itemValueSize(void* item)
{
    return readSize((char*)item + sizeof(size_t));
}

void* Table::itemKey(void* item)
{
    return (char*)item + HEADER_SIZE;
}

void* Table::itemValue(void* item)
{
    return (char*)item + HEADER_SIZE + itemKeySize(item);
}

size_t Table::hashKey(void* key, size_t keySize)
{
    unsigned char* p;
    size_t h;
    size_t i;

    p = (unsigned char*)key;
    h = 0;

    for (i = 0; i < keySize; i++)
        h = h * 131 + p[i];

    return h;
}

bool Table::isPrime(size_t n)
{
    size_t d;

    if (n < 2)
        return 0;

    if (n == 2)
        return 1;

    if (n % 2 == 0)
        return 0;

    for (d = 3; d * d <= n; d += 2)
    {
        if (n % d == 0)
            return 0;
    }

    return 1;
}

size_t Table::nextPrime(size_t n)
{
    while (isPrime(n) == 0)
        n++;

    return n;
}

size_t Table::nextBucketCount(size_t current)
{
    size_t target;

    target = current * 2;
    if (target <= current)
        target = current + 1;

    return nextPrime(target);
}

bool Table::needsRehash() const
{
    return _size * MAX_LOAD_DEN > _bucketCount * MAX_LOAD_NUM;
}

int Table::rehash()
{
    AbstractList** oldBuckets;
    size_t oldCount;
    size_t newCount;
    AbstractList** newBuckets;
    size_t b;
    size_t i;
    Iterator* it;
    size_t itemSize;
    void* item;
    size_t newBucket;

    oldBuckets = _buckets;
    oldCount = _bucketCount;
    newCount = nextBucketCount(oldCount);

    if (newCount <= oldCount)
        return 0;

    newBuckets = (AbstractList**)_memory.allocMem(sizeof(AbstractList*) * newCount);
    if (newBuckets == 0)
        return 1;

    for (i = 0; i < newCount; i++)
    {
        newBuckets[i] = _factory(_memory);
        if (newBuckets[i] == 0)
        {
            for (b = 0; b < i; b++)
                delete newBuckets[b];

            _memory.freeMem(newBuckets);
            return 1;
        }
    }

    _buckets = newBuckets;
    _bucketCount = newCount;

    for (b = 0; b < oldCount; b++)
    {
        it = oldBuckets[b]->newIterator();
        while (it)
        {
            itemSize = 0;
            item = it->getElement(itemSize);
            if (item == 0)
                break;

            newBucket = bucketIndex(itemKey(item), itemKeySize(item));
            if (_buckets[newBucket]->push_front(item, itemSize) != 0)
            {
                delete it;
                return 1;
            }

            oldBuckets[b]->remove(it);

            itemSize = 0;
            if (it->getElement(itemSize) == 0 && it->hasNext() == 0)
                break;
        }

        delete it;
    }

    for (b = 0; b < oldCount; b++)
        delete oldBuckets[b];

    _memory.freeMem(oldBuckets);
    return 0;
}

size_t Table::bucketIndex(void* key, size_t keySize) const
{
    return hashKey(key, keySize) % _bucketCount;
}

Container::Iterator* Table::findRawByKey(size_t bucket, void* key, size_t keySize)
{
    Iterator* it;
    size_t itemSize;
    void* item;

    if (_buckets[bucket]->empty())
        return 0;

    it = _buckets[bucket]->newIterator();
    while (it)
    {
        itemSize = 0;
        item = it->getElement(itemSize);

        if (item &&
            itemKeySize(item) == keySize &&
            (keySize == 0 || memcmp(itemKey(item), key, keySize) == 0))
        {
            return it;
        }

        if (it->hasNext() == 0)
            break;

        it->goToNext();
    }

    delete it;
    return 0;
}

void Table::moveIteratorToNextBucket(TableIterator* it)
{
    size_t b;

    delete it->inner;
    it->inner = 0;

    for (b = it->bucket + 1; b < _bucketCount; b++)
    {
        if (_buckets[b]->empty() == 0)
        {
            it->bucket = b;
            it->inner = _buckets[b]->newIterator();
            return;
        }
    }

    it->bucket = _bucketCount;
}

int Table::insertByKey(void* key, size_t keySize, void* elem, size_t elemSize)
{
    size_t bucket;
    Iterator* found;
    char* item;
    int result;

    if ((keySize && !key) || (elemSize && !elem))
        return 1;

    bucket = bucketIndex(key, keySize);
    found = findRawByKey(bucket, key, keySize);
    if (found)
    {
        delete found;
        return 1;
    }

    item = makeItem(key, keySize, elem, elemSize);
    if (item == 0)
        return 1;

    result = _buckets[bucket]->push_front(item, HEADER_SIZE + keySize + elemSize);
    _memory.freeMem(item);

    if (result != 0)
        return 1;

    _size++;
    if (needsRehash())
        rehash();

    return 0;
}

void Table::removeByKey(void* key, size_t keySize)
{
    size_t bucket;
    Iterator* it;

    bucket = bucketIndex(key, keySize);
    it = findRawByKey(bucket, key, keySize);

    if (it == 0)
        return;

    _buckets[bucket]->remove(it);
    delete it;
    _size--;
}

Container::Iterator* Table::findByKey(void* key, size_t keySize)
{
    size_t bucket;
    Iterator* it;

    bucket = bucketIndex(key, keySize);
    it = findRawByKey(bucket, key, keySize);

    if (it == 0)
        return 0;

    return new TableIterator(this, bucket, it);
}

void* Table::at(void* key, size_t keySize, size_t& valueSize)
{
    Iterator* it;
    void* result;

    it = findByKey(key, keySize);
    if (it == 0)
    {
        valueSize = 0;
        return 0;
    }

    result = it->getElement(valueSize);
    delete it;
    return result;
}

int Table::size()
{
    return (int)_size;
}

size_t Table::max_bytes()
{
    return _memory.maxBytes();
}

Container::Iterator* Table::find(void* elem, size_t elemSize)
{
    size_t b;
    Iterator* it;
    size_t itemSize;
    void* item;

    for (b = 0; b < _bucketCount; b++)
    {
        if (_buckets[b]->empty())
            continue;

        it = _buckets[b]->newIterator();
        while (it)
        {
            itemSize = 0;
            item = it->getElement(itemSize);

            if (item &&
                itemValueSize(item) == elemSize &&
                (elemSize == 0 || memcmp(itemValue(item), elem, elemSize) == 0))
            {
                return new TableIterator(this, b, it);
            }

            if (it->hasNext() == 0)
                break;

            it->goToNext();
        }

        delete it;
    }

    return 0;
}

Container::Iterator* Table::newIterator()
{
    size_t b;

    for (b = 0; b < _bucketCount; b++)
    {
        if (_buckets[b]->empty() == 0)
            return new TableIterator(this, b, _buckets[b]->newIterator());
    }

    return 0;
}

void Table::remove(Iterator* iter)
{
    TableIterator* it;
    size_t itemSize;

    if (iter == 0)
        return;

    it = (TableIterator*)iter;
    if (it->table != this || it->bucket >= _bucketCount || it->inner == 0)
        return;

    _buckets[it->bucket]->remove(it->inner);
    _size--;

    itemSize = 0;
    if (it->inner->getElement(itemSize) == 0)
        moveIteratorToNextBucket(it);
}

void Table::clear()
{
    size_t i;

    for (i = 0; i < _bucketCount; i++)
    {
        _buckets[i]->clear();
    }

    _size = 0;
}

bool Table::empty()
{
    return _size == 0;
}

int Table::insert_by_key(KV* elem, size_t elemSize)
{
    if (!elem || elemSize != sizeof(KV))
        return 1;

    return insertByKey(&elem->key, sizeof(elem->key), elem, elemSize);
}

int Table::insert_string(KV_str* elem, size_t elemSize)
{
    if (!elem || elemSize != sizeof(KV_str) || !elem->key)
        return 1;

    return insertByKey((void*)elem->key, strlen(elem->key) + 1, elem, elemSize);
}

void Table::print()
{
    Iterator* it;
    size_t s;
    KV* item;

    it = newIterator();
    while (it)
    {
        s = 0;
        item = (KV*)it->getElement(s);
        if (item && s == sizeof(KV))
            printf("%d: %d\n", item->key, item->value);

        if (it->hasNext() == 0)
        {
            delete it;
            break;
        }

        it->goToNext();
    }
}

void Table::print_string()
{
    Iterator* it;
    size_t s;
    KV_str* item;

    it = newIterator();
    while (it)
    {
        s = 0;
        item = (KV_str*)it->getElement(s);
        if (item && s == sizeof(KV_str))
            printf("%s: %s\n", item->key, item->value);

        if (it->hasNext() == 0)
        {
            delete it;
            break;
        }

        it->goToNext();
    }
}
