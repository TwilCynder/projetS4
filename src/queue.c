#include <stdlib.h>
#include "queue.h"

typedef struct s_queueElt{
    void* value;
    struct s_queueElt* next;
} QueueElt;

struct s_queue {
    int size;
    QueueElt* head;
    QueueElt* tail;
};

Queue* create_queue(){
    Queue* q = malloc(sizeof(Queue));
    q->size = 0;
    q->head = NULL;
    q->tail = NULL;
    return q;
}

void queue_push(Queue *q, void *value) {
    QueueElt *new = malloc(sizeof(QueueElt));
    QueueElt **insert_at = (q->size ? &(q->tail->next) : &(q->head));
    new->value = value;
    new->next = NULL;
    *insert_at = new;
    q->tail = new;
    q->size++;
}

void queue_pop(Queue *q) {
    QueueElt *old = q->head;
    q->head = q->head->next;
    if (q->head == NULL) q->tail = NULL;
    q->size--;
    free(old);
}

void* queue_top(Queue* q){
    return q->head->value;
}

int queue_size(Queue* q){
    return q->size;
}