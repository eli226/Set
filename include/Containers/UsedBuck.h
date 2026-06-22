#pragma once
#ifndef SET_USEDBUCK_H
#define SET_USEDBUCK_H

#include "AbstractClasses/ListAbstract.h"

class UsedBuck
{
private:
    struct used
    {
        size_t index;
        AbstractList* bucket;
        used* next;
    };

    used* head;
    size_t count;

public:
    UsedBuck() : head(nullptr), count(0) {}

    ~UsedBuck() {
        clear();
    }

    void clear() {
        while (head != nullptr) {
            used* tmp = head;
            head = head->next;
            delete tmp;
        }
        count = 0;
    }

    void newNode(AbstractList* bucket, size_t index) {
        used* node = new used;
        node->bucket = bucket;
        node->index = index;
        node->next = head;
        head = node;
        ++count;
    }

    int remove(AbstractList* buck) {
        used* curr = head;
        used* prev = nullptr;

        while (curr != nullptr && curr->bucket != buck) {
            prev = curr;
            curr = curr->next;
        }

        if (curr == nullptr)
            return -1;

        if (prev == nullptr) {
            head = curr->next;
        } else {
            prev->next = curr->next;
        }

        delete curr;
        --count;
        return 0;
    }

    size_t get_count() const {
        return count;
    }

    size_t firstIndex() const {
        if (head == nullptr)
            return (size_t)-1;

        size_t best = (size_t)-1;
        for (used* curr = head; curr != nullptr; curr = curr->next) {
            if (curr->index < best)
                best = curr->index;
        }
        return best;
    }

    size_t nextAfter(size_t index) const {
        size_t best = (size_t)-1;

        for (used* curr = head; curr != nullptr; curr = curr->next) {
            if (curr->index > index && curr->index < best)
                best = curr->index;
        }

        return best;
    }

    bool hasAfter(size_t index) const {
        return nextAfter(index) != (size_t)-1;
    }
};

#endif // SET_USEDBUCK_H