/**
 * @file queue.h
 * @brief A simple singly-linked list with FIFO capabilities.
 * Holds void* values, whiwh was not really necessary since we are only using this structure for the transaction queue.
 * @copyright Copyright (c) 2021
 * 
 */

typedef struct s_queue Queue;

Queue* create_queue();
void queue_push(Queue* q, void* value);
void queue_pop(Queue* q);
void* queue_top(Queue* q);
int queue_size(Queue* q);
