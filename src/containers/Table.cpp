/*#include "Table.h"
#include <cstring>
#include "List2.h"

Table::Table(MemoryManager &mem) : AbstractTable(mem)
{
    capacity = 101; 
    count = 0;
    void* raw_mem = _memory.allocMem(sizeof(List2) * capacity);
    buckets = ::new (raw_mem) List2[capacity];
}

Table::~Table() 
{
    clear();
    _memory.freeMem(buckets);
}

size_t Table::get_hash(void* data, size_t size) 
{
    size_t hash_value = 0;
    unsigned char* bytes = (unsigned char*)data;

    for (size_t i = 0; i < size; i++) 
    {
        hash_value += bytes[i];
    }
    return hash_value % capacity;
}

size_t Table::get_hash_by_key(void* data) 
{
    KV* kv = (KV*)data;
    return kv->key % capacity;
}

// Отладка на строках
size_t Table::get_hash_string(void* data) 
{
    KV_str* kv = (KV_str*)data;
    size_t hash_value = 0;

    for (char c : kv->key) 
    {
        hash_value += c;
    }
    return hash_value % capacity;
}

void Table::insert(void* elem, size_t size, void* key, size_t key_size)
{
    size_t index = get_hash(elem, size);

     new_item;
    new_item.data_size = size;

    new_item.data_ptr = _memory.allocMem(size);
    memcpy(new_item.data_ptr, elem, size);

    buckets[index].push_back(new_item);
    count++;
}

void Table::insert_by_key(void* elem, size_t size) 
{
    size_t index = get_hash_by_key(elem);

    TableItem new_item;
    new_item.data_size = size;

    new_item.data_ptr = _memory.allocMem(size);
    memcpy(new_item.data_ptr, elem, size);

    buckets[index].push_back(new_item);
    count++;
}

// Отладка на строках
void Table::insert_string(void* elem, size_t size) 
{
    size_t index = get_hash_string(elem);

    TableItem new_item;
    new_item.data_size = size;

    new_item.data_ptr = _memory.allocMem(size);
    memcpy(new_item.data_ptr, elem, size);

    buckets[index].push_back(new_item);
    count++;
}

int Table::size() 
{
    return count;
}

size_t Table::max_bytes() 
{
    return _memory.size();
}

bool Table::empty() 
{
    return count == 0;
}

void Table::clear() 
{
    for (size_t i = 0; i < capacity; i++) 
    {
        for (auto& item : buckets[i]) 
        {
            _memory.freeMem(item.data_ptr);
        }
        buckets[i].clear();
    }
    count = 0;
}

void Table::print() const
{
    for (size_t i = 0; i < capacity; ++i)
    {
        std::cout << "[" << i << "]";

        for (const auto& item : buckets[i])
        {
            const KV* p = static_cast<const KV*>(item.data_ptr);
            std::cout << " -> " << p->value << " (key: " << p->key << ")";
        }

        std::cout << '\n';
    }
}

void Table::print_string() const
{
    for (size_t i = 0; i < capacity; ++i)
    {
        std::cout << "[" << i << "]";

        for (const auto& item : buckets[i])
        {
            const KV_str* p = static_cast<const KV_str*>(item.data_ptr);
            std::cout << " -> " << p->value << " (key: " << p->key << ")";
        }

        std::cout << '\n';
    }
}

// Заглушки
Container::Iterator* Table::find(void* elem, size_t size) { return nullptr; }
Container::Iterator* Table::newIterator() { return nullptr; }
void Table::remove(Iterator* iter) { }*/