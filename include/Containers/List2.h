#pragma once
#include "List2Abstract.h"
#include "GroupList.h"

class List2 : public AbstractList2
{
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

    Iterator *find(void *elem, size_t size) override;

    Iterator *newIterator() override;

    void remove(Iterator *iter) override;

    void clear() override;

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

        Node *get_previous(Node *node)
        {
            return current->previous;
        }
        
        void *getElement(size_t &size) override;
        bool hasNext() override;
        void goToNext() override;
        bool equals(Iterator *right) override;
    };

    friend class ListIterator;
};
