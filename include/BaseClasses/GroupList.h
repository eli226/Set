#pragma once
#include "Container.h"

// Базовый класс для однонаправленного и двунаправленного списков
class GroupList : public Container
{
public:
    size_t _size;
    size_t _global_size;
    int _elem_quantity;
    
    GroupList(MemoryManager &mem) : Container(mem) {}

    int size() override;

    size_t max_bytes() override;

    bool empty() override;

    virtual ~GroupList() {}
};