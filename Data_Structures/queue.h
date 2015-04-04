#ifndef QUEUE_H_
#define QUEUE_H_

typedef struct queue_t QUEUE;

QUEUE* init_queue(void(*ufree)(void*));
void enqueue(QUEUE* q,void* data);
void* dequeue(QUEUE* q);
void* peek_queue(QUEUE* q);
/* Returns 1 if empty 0 otherwise */
int is_queue_empty(QUEUE* q);
void destroy_queue(QUEUE* q);

#endif
