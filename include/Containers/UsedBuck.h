#ifndef SET_USEDBUCK_H
#define SET_USEDBUCK_H
#include "List2.h"

class UsedBuck {
    typedef struct used {
        used* next;
        List2* bucket;
    }used;
    used* head;
    size_t count;

    public:
    UsedBuck() {
        head = nullptr;
        count = 0;
    }
    ~UsedBuck() {
        used* tmp;
        while (head != nullptr) {
            tmp = head;
            head = head->next;
            delete tmp;
        }
    }
    void newNode(List2* index) { //allocate нового узла
        used* node = new used;
        node->bucket = index;
        node->next = head;
        head = node;
        count++;
    }
    int remove(List2* buck) {// return -1 - ошибка выполнения // return 0 - успех
        used* curr = head;
        used* prev = nullptr;

        while (curr != nullptr && curr->bucket != buck) {
            prev = curr;
            curr = curr->next;
        }
        if (curr == nullptr) return -1;

        if (prev == nullptr) {
            head = head->next;
            delete curr;
            count--;
            return 0;
        }
        else {
            prev->next = curr->next;
            delete curr;
            count--;
            return 0;
        }
        return -1;
    }
    size_t get_count() const {
        return count;
    }
};
#endif //SET_USEDBUCK_H