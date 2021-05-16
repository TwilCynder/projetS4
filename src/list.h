#pragma once

typedef struct s_list List;
typedef struct s_Iterator Iterator;

typedef void (*MapFunctor)(void* value);

List* create_list();
void list_push(List* l, void* value);
void* list_front(List* l);
int list_size(List* l);
void list_reduce(List* l, MapFunctor f);
void list_delete(List* l);

Iterator* create_iterator(List* l);
void iterator_next(Iterator* it);
int iterator_end(Iterator* it);
void* iterator_value(Iterator* it);
void iterator_delete(Iterator* it);