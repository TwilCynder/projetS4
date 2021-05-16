#include "list.h"
#include <stdlib.h>

typedef struct s_listElt {
    void* value;
    struct s_listElt* previous;
    struct s_listElt* next;
} ListElt;

struct s_list {
    ListElt* sentinel;
    int size; 
};

struct s_Iterator {
    List* list;
    ListElt* current;
};

List* create_list(){
    List* l = malloc(sizeof(List));
    l->sentinel = malloc(sizeof(ListElt));
    l->sentinel->previous = l->sentinel;
    l->sentinel->next = l->sentinel;
    l->size = 0;

    return l;
}

void list_push(List* l, void* value){
    ListElt* elt = malloc(sizeof(ListElt));
    elt->value = value;
    l->sentinel->previous->next = elt;
    elt->previous = l->sentinel->previous;
    elt->next = l->sentinel;
    l->sentinel->previous = elt;
    l->size++;
}

void* list_front(List* l){
    if (!l->size) return NULL;
    return l->sentinel->previous->value;
}

int list_size(List* l){
    return l->size;
}

void list_reduce(List* l, MapFunctor f){
    ListElt* current = l->sentinel->next;

    while (current != l->sentinel){
        f(current->value);
        current = current->next;
    }
}

void list_delete(List* l){
    ListElt* current = l->sentinel->next;
    ListElt* next;

    while (current != l->sentinel){ 
        next = current->next;
        free(current);
        current = next;
    }
}

Iterator* create_iterator(List* l){
    Iterator* it = malloc(sizeof(Iterator));
    it->list = l;
    it->current = l->sentinel->next;
    return it;
}

void iterator_next(Iterator* it){
    it->current = it->current->next;
}

int iterator_end(Iterator* it){
    return it->current == it->list->sentinel;
}

void iterator_delete(Iterator* it){
    free(it);
}

void* iterator_value(Iterator* it){
    return it->current->value;
}