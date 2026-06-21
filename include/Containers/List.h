#pragma once
#include "AbstractClasses/ListAbstract.h"
#include "MM/MemoryManager.h"
class List : public AbstractList
{
private:
    struct node
    {
        size_t size; 
        void* data;
        node* next; 
    };
     
    size_t global_elems; 
    size_t global_size;
    node* head;   
public:
    
    List(MemoryManager& mem);
    ~List();
    int push_front(void* elem, size_t elemSize) override;
    void pop_front() override;
    void* front(size_t& size) override;
    int insert(Iterator* iter, void* elem, size_t elemSize) override;

    class Iterator_list : public Iterator
    {
     public:
         Iterator_list(node* h) :curr(h) {};
         void* getElement(size_t& size)  override;
         bool hasNext()  override;
         void goToNext()  override;
         bool equals(Iterator* right) override;
         node* curr;
         node* prev;
    
    };
    int size() override;
    size_t max_bytes() override;
     Iterator* find(void* elem, size_t size) override; 
     Iterator* newIterator() override;
     void remove(Iterator* iter) override;
     void clear() override;
     bool empty() override;
};


