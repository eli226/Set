#ifndef SET_USEDBUCK_H
#define SET_USEDBUCK_H
#include "List2.h"

class UsedBuck {
    typedef struct used{
        used* next;
        List2* bucket;
    }used;
    used* head;

    public:
    UsedBuck() {
        head = nullptr;
    }
    void newNode(List2* index) {
        used* node = new used;
        node->bucket = index;
        node->next = head;
        head = node;
    }
    int remove(List2* buck) {// return -1 - ошибка выполнения // return 0 - успех
        used* curr = head;
        used* prev = nullptr;
        if (node == head) {// когда node первый элемент
            head = head->next;
            delete curr;
            return 0;
        }
        else if (node->next == nullptr) { //когда node последний элемент
            while (curr->bucket != buck) {
                prev = curr;
                curr = curr->next;
            }
            prev->next = nullptr;
            delete curr;
            return 0;
        }
        else { //когда node находится между двумя узлами
            while (curr->bucket != buck) {
                prev = curr;
                curr = curr->next;
            }
            prev->next = curr->next;
            delete curr;
            return 0;
        }
        return -1;
    }
};

#endif //SET_USEDBUCK_H