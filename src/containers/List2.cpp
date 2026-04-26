#include <iostream>
#include "../../include/Containers/List2.h"

int List2::push_back(void *elem, size_t elemSize)
{
    if (_size >= elemSize + sizeof(Node))
    {
        Node *node = (Node *)_memory.allocMem(sizeof(Node));

        if (node)
        {
            node->data = _memory.allocMem(elemSize);
            if (node->data == nullptr)
            {
                _memory.freeMem(node);
                return 1;
            }

            memcpy(node->data, elem, elemSize);
            node->node_size = elemSize;

            if (_tail)
            {
                node->previous = _tail;
                node->next = nullptr;
                _tail->next = node;
            }
            else
            {
                node->previous = nullptr;
                node->next = nullptr;
                _head = node;
            }

            _tail = node;
            _size -= (elemSize + sizeof(Node));
            _elem_quantity++;

            return 0;
        }

        return 1;
    }

    return 1;
}

int List2::push_front(void *elem, size_t elemSize)
{
    if (_size >= elemSize + sizeof(Node))
    {
        Node *node = (Node *)_memory.allocMem(sizeof(Node));
        if (node)
        {
            node->data = _memory.allocMem(elemSize);
            if (node->data == nullptr)
            {
                _memory.freeMem(node);
                return 1;
            }
            memcpy(node->data, elem, elemSize);
            node->node_size = elemSize;

            if (_head)
            {
                node->previous = nullptr;
                node->next = _head;
                _head->previous = node;
            }
            else
            {
                node->previous = nullptr;
                node->next = nullptr;
                _tail = node;
            }

            _head = node;
            _size -= (elemSize + sizeof(Node));
            _elem_quantity++;

            return 0;
        }

        return 1;
    }

    return 1;
}

void List2::pop_back()
{
    if (!_tail)
        return;

    Node *node = _tail;
    _tail = _tail->previous;

    if (_tail)
        _tail->next = nullptr;
    else
        _head = nullptr;

    size_t free_memory = node->node_size;
    _memory.freeMem(node->data);
    _memory.freeMem(node);

    _elem_quantity--;
    _size += free_memory + sizeof(Node);
}

void List2::pop_front()
{
    if (!_head)
        return;

    Node *node = _head;
    _head = _head->next;

    if (_head)
        _head->previous = nullptr;
    else
        _tail = nullptr;

    size_t free_memory = node->node_size;
    _memory.freeMem(node->data);
    _memory.freeMem(node);

    _elem_quantity--;
    _size += free_memory + sizeof(Node);
}

void *List2::front(size_t &size)
{
    if (!_head)
    {
        size = 0;
        return nullptr;
    }

    size = _head->node_size;
    return _head->data;
}

void *List2::back(size_t &size)
{
    if (!_tail)
    {
        size = 0;
        return nullptr;
    }

    size = _tail->node_size;
    return _tail->data;
}

// Container

int List2::insert(Iterator *iter, void *elem, size_t elemSize)
{
    if (_size < elemSize + sizeof(Node))
        return 1;

    Node *node = (Node *)_memory.allocMem(sizeof(Node));

    if (!node)
        return 1;

    node->data = _memory.allocMem(elemSize);

    if (!node->data)
    {
        _memory.freeMem(node);
        return 1;
    }

    memcpy(node->data, elem, elemSize);
    node->node_size = elemSize;

    ListIterator *iterator = dynamic_cast<ListIterator *>(iter);

    Node *current = (iterator ? iterator->get_current() : nullptr);

    if (!current)
    {
        node->next = nullptr;
        node->previous = _tail;

        if (_tail)
            _tail->next = node;
        else
            _head = node;

        _tail = node;
    }
    else
    {
        node->next = current;
        node->previous = current->previous;

        if (current->previous)
            current->previous->next = node;
        else
            _head = node;

        current->previous = node;
    }

    _elem_quantity++;
    _size -= elemSize + sizeof(Node);

    return 0;
}

int List2::size()
{
    return _elem_quantity;
}

size_t List2::max_bytes()
{
    return _global_size;
}

Container::Iterator *List2::find(void *elem, size_t size)
{
    Node *current = _head;

    while (current)
    {
        if (current->node_size == size && memcmp(current->data, elem, size) == 0)
        {
            return new ListIterator(current);
        }
        current = current->next;
    }

    return nullptr;
}

Container::Iterator *List2::newIterator()
{
    return new ListIterator(_head);
}

void List2::remove(Iterator *iter)
{
    if (!iter)
        return;

    ListIterator *iterator = dynamic_cast<ListIterator *>(iter);

    if (!iterator)
        return;

    Node *node = iterator->get_current();

    if (!node)
        return;

    Node *next_node = node->next;

    if (node->previous)
        node->previous->next = node->next;
    else
        _head = node->next;

    if (node->next)
        node->next->previous = node->previous;
    else
        _tail = node->previous;

    _memory.freeMem(node->data);
    _memory.freeMem(node);

    _elem_quantity--;
    _size += node->node_size + sizeof(Node);

    iterator->set_current(next_node);
}

void List2::clear()
{
    Node *next = nullptr;

    while (_head)
    {
        _memory.freeMem(_head->data);
        next = _head->next;
        _memory.freeMem(_head);
        _head = next;
    }

    _tail = nullptr;
    _elem_quantity = 0;
    _size = _global_size;
}

bool List2::empty()
{
    return _elem_quantity == 0;
}

void *List2::ListIterator::getElement(size_t &size)
{
    if (!current)
        return nullptr;

    size = current->node_size;
    return current->data;
}

bool List2::ListIterator::hasNext()
{
    return current != nullptr && current->next != nullptr;
}

void List2::ListIterator::goToNext()
{
    if (current)
        current = current->next;
}

bool List2::ListIterator::equals(Iterator *right)
{
    if (!right)
        return false;

    ListIterator *iterator = dynamic_cast<ListIterator*>(right);
    if (!iterator)
        return false;

    return current == iterator->current;
}