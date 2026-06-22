#include <iostream>
#include "GroupList.h"

int GroupList::size()
{
    return _elem_quantity;
}

size_t GroupList::max_bytes()
{
    return _global_size;
}

bool GroupList::empty()
{
    return _elem_quantity == 0;
}