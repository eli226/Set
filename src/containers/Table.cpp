#include "Containers/Table.h"
#include "Containers/List.h"
#include <cstring>
#include <iostream>

Container::Iterator::~Iterator() {}

static AbstractList* createDefaultBucket(MemoryManager& mem)
{
    return new List(mem);
}

class Table::TableIterator : public Iterator
{
public:
    TableIterator(Table* t, size_t b, Iterator* i) : table(t), bucket(b), inner(i) {}

    ~TableIterator() override
    {
        delete inner;
    }

    void* getElement(size_t& size) override
    {
        if (!inner)
        {
            size = 0;
            return 0;
        }

        size_t itemSize = 0;
        void* item = inner->getElement(itemSize);
        if (!item)
        {
            size = 0;
            return 0;
        }

        size = itemValueSize(item);
        return itemValue(item);
    }

    bool hasNext() override
    {
        if (inner && inner->hasNext())
            return true;

        for (size_t i = bucket + 1; i < table->_bucketCount; ++i)
            if (!table->_buckets[i]->empty())
                return true;

        return false;
    }

    void goToNext() override
    {
        if (inner && inner->hasNext())
        {
            inner->goToNext();
            return;
        }

        table->moveIteratorToNextBucket(this);
    }

    bool equals(Iterator* right) override
    {
        TableIterator* r = dynamic_cast<TableIterator*>(right);
        if (!r || table != r->table || bucket != r->bucket)
            return false;

        if (!inner || !r->inner)
            return inner == r->inner;

        return inner->equals(r->inner);
    }

    Table* table;
    size_t bucket;
    Iterator* inner;
};

Table::Table(MemoryManager& mem, size_t bucketCount, BucketFactory factory)
    : AbstractTable(mem),
      _buckets(0),
      _bucketCount(bucketCount ? bucketCount : 1),
      _size(0),
      _factory(factory ? factory : createDefaultBucket)
{
    _buckets = new AbstractList*[_bucketCount];
    for (size_t i = 0; i < _bucketCount; ++i)
        _buckets[i] = _factory(_memory);
}

Table::~Table()
{
    clear();

    for (size_t i = 0; i < _bucketCount; ++i)
        delete _buckets[i];

    delete[] _buckets;
}

void Table::writeSize(void* ptr, size_t value)
{
    std::memcpy(ptr, &value, sizeof(size_t));
}

size_t Table::readSize(void* ptr)
{
    size_t value = 0;
    std::memcpy(&value, ptr, sizeof(size_t));
    return value;
}

char* Table::makeItem(void* key, size_t keySize, void* elem, size_t elemSize)
{
    char* item = new char[HEADER_SIZE + keySize + elemSize];

    writeSize(item, keySize);
    writeSize(item + sizeof(size_t), elemSize);

    if (keySize)
        std::memcpy(item + HEADER_SIZE, key, keySize);
    if (elemSize)
        std::memcpy(item + HEADER_SIZE + keySize, elem, elemSize);

    return item;
}

size_t Table::itemKeySize(void* item)
{
    return readSize(item);
}

size_t Table::itemValueSize(void* item)
{
    return readSize(static_cast<char*>(item) + sizeof(size_t));
}

void* Table::itemKey(void* item)
{
    return static_cast<char*>(item) + HEADER_SIZE;
}

void* Table::itemValue(void* item)
{
    return static_cast<char*>(item) + HEADER_SIZE + itemKeySize(item);
}

size_t Table::bucketIndex(void* key, size_t keySize) const
{
    unsigned char* p = static_cast<unsigned char*>(key);
    size_t h = 0;

    for (size_t i = 0; i < keySize; ++i)
        h = h * 131 + p[i];

    return h % _bucketCount;
}

Container::Iterator* Table::findRawByKey(size_t bucket, void* key, size_t keySize)
{
    if (_buckets[bucket]->empty())
        return 0;

    Iterator* it = _buckets[bucket]->newIterator();
    while (it)
    {
        size_t itemSize = 0;
        void* item = it->getElement(itemSize);

        if (item &&
            itemKeySize(item) == keySize &&
            (keySize == 0 || std::memcmp(itemKey(item), key, keySize) == 0))
            return it;

        if (!it->hasNext())
            break;

        it->goToNext();
    }

    delete it;
    return 0;
}

void Table::moveIteratorToNextBucket(TableIterator* it)
{
    delete it->inner;
    it->inner = 0;

    for (size_t b = it->bucket + 1; b < _bucketCount; ++b)
    {
        if (!_buckets[b]->empty())
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
    if ((keySize && !key) || (elemSize && !elem))
        return 1;

    size_t bucket = bucketIndex(key, keySize);
    Iterator* found = findRawByKey(bucket, key, keySize);
    if (found)
    {
        delete found;
        return 1;
    }

    char* item = makeItem(key, keySize, elem, elemSize);
    int result = _buckets[bucket]->push_front(item, HEADER_SIZE + keySize + elemSize);
    delete[] item;

    if (result != 0)
        return 1;

    ++_size;
    return 0;
}

void Table::removeByKey(void* key, size_t keySize)
{
    size_t bucket = bucketIndex(key, keySize);
    Iterator* it = findRawByKey(bucket, key, keySize);

    if (!it)
        return;

    _buckets[bucket]->remove(it);
    delete it;
    --_size;
}

Container::Iterator* Table::findByKey(void* key, size_t keySize)
{
    size_t bucket = bucketIndex(key, keySize);
    Iterator* it = findRawByKey(bucket, key, keySize);

    if (!it)
        return 0;

    return new TableIterator(this, bucket, it);
}

void* Table::at(void* key, size_t keySize, size_t& valueSize)
{
    Iterator* it = findByKey(key, keySize);
    if (!it)
    {
        valueSize = 0;
        return 0;
    }

    void* result = it->getElement(valueSize);
    delete it;
    return result;
}

int Table::size()
{
    return static_cast<int>(_size);
}

size_t Table::max_bytes()
{
    return _memory.maxBytes();
}

Container::Iterator* Table::find(void* elem, size_t elemSize)
{
    for (size_t b = 0; b < _bucketCount; ++b)
    {
        if (_buckets[b]->empty())
            continue;

        Iterator* it = _buckets[b]->newIterator();
        while (it)
        {
            size_t itemSize = 0;
            void* item = it->getElement(itemSize);

            if (item &&
                itemValueSize(item) == elemSize &&
                (elemSize == 0 || std::memcmp(itemValue(item), elem, elemSize) == 0))
                return new TableIterator(this, b, it);

            if (!it->hasNext())
                break;

            it->goToNext();
        }

        delete it;
    }

    return 0;
}

Container::Iterator* Table::newIterator()
{
    for (size_t b = 0; b < _bucketCount; ++b)
        if (!_buckets[b]->empty())
            return new TableIterator(this, b, _buckets[b]->newIterator());

    return nullptr;
}

void Table::remove(Iterator* iter)
{
    TableIterator* it = dynamic_cast<TableIterator*>(iter);
    if (!it || it->table != this || it->bucket >= _bucketCount || !it->inner)
        return;

    _buckets[it->bucket]->remove(it->inner);
    --_size;

    size_t itemSize = 0;
    if (!it->inner->getElement(itemSize))
        moveIteratorToNextBucket(it);
}

void Table::clear()
{
    for (size_t i = 0; i < _bucketCount; ++i)
        _buckets[i]->clear();

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

    return insertByKey((void*)elem->key, std::strlen(elem->key) + 1, elem, elemSize);
}

void Table::print()
{
    for (Iterator* it = newIterator(); it; )
    {
        size_t s = 0;
        KV* item = static_cast<KV*>(it->getElement(s));
        if (item && s == sizeof(KV))
            std::cout << item->key << ": " << item->value << std::endl;

        if (!it->hasNext())
        {
            delete it;
            break;
        }

        it->goToNext();
    }
}

void Table::print_string()
{
    for (Iterator* it = newIterator(); it; )
    {
        size_t s = 0;
        KV_str* item = static_cast<KV_str*>(it->getElement(s));
        if (item && s == sizeof(KV_str))
            std::cout << item->key << ": " << item->value << std::endl;

        if (!it->hasNext())
        {
            delete it;
            break;
        }

        it->goToNext();
    }
}
