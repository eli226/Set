#pragma once
#include "AbstractClasses/List2Abstract.h"
#include "BaseClasses/GroupList.h"

class List2 final : public AbstractList2
{
    friend class Set;
private:
    typedef struct Node
    {
        Node *next;
        Node *previous;
        size_t node_size;
        void *data;
    } Node;

    Node *_head;
    Node *_tail;
    size_t _size;
    size_t _global_size;
    int _elem_quantity;

public:
    List2(MemoryManager &mem) : AbstractList2(mem)
    {
        _head = nullptr;
        _tail = nullptr;
        _global_size = mem.size();
        _size = _global_size;
        _elem_quantity = 0;
    }

    ~List2()
    {
        clear();
        _size = 0;
    }

    int push_back(void *elem, size_t elemSize) override;

    int push_front(void *elem, size_t elemSize) override;

    void pop_back() override;

    void pop_front() override;

    void *front(size_t &size) override;

    void *back(size_t &size) override;
    
    int insert(Iterator *iter, void *elem, size_t elemSize) override;

    // Container

    int size() override;

    size_t max_bytes() override;

    Iterator *find(void *elem, size_t size) override;

    Iterator *newIterator() override;

    void remove(Iterator *iter) override;

    void clear() override;

    bool empty() override;

    class ListIterator : public Iterator
    {
    private:
        Node *current;

    public:
        ListIterator(Node *node) : current(node) {}

        Node *get_current()
        {
            return current;
        }

        void set_current(Node *node)
        {
            current = node;
        }
        
        void *getElement(size_t &size) override;
        bool hasNext() override;
        void goToNext() override;
        bool equals(Iterator *right) override;

    };
    friend class ListIterator;
};