#ifndef SET_MEMPOOL_H
#define SET_MEMPOOL_H
#include "../MemoryManager.h"

class memPool : public MemoryManager{
    size_t _size;
    char* buff;
    char* ptr;
    char* end;
public:
    memPool(size_t size) : MemoryManager(size) {
        char* buff = new char[size];
        ptr = buff;
        end = buff + size;
    }

    size_t maxBytes() override;
    void* allocMem(size_t sz) override;
    void freeMem(void *ptr) override;
};

#endif //SET_MEMPOOL_H