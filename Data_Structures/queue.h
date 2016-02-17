#ifndef QUEUE_H_
#define QUEUE_H_

typedef struct queue_t QUEUE;

/**
 * Creates an empty Queue
 * @param myfree - function pointer to free user data
 * @return QUEUE* on success, NULL on failure
 */
QUEUE* init_queue(void (*ufree)(void*));

/**
 * Adds an element to the QUEUE
 * @param QUEUE* q - the Queue to use
 * @param void* data - The data to add
 * @return -1 if QUEUE is NULL, -2 if node malloc fails
 * 0 on success
 */
int enqueue(QUEUE* q, void* data);

/**
 * Removes an element from the queue and returns pointer
 * @param QUEUE* q - The queue to use
 * @return NULL if QUEUE is NULL or empty,
 * a pointer to the data otherwise
 */
void* dequeue(QUEUE* q);

/**
 * peek at the top element
 * @param QUEUE* q - The queue to use
 * @return NULL if the Queue is NULL or empty
 * a pointer to the data otherwise.
 */
void* peek_queue(QUEUE* q);

/**
 * Function to see of the queue is empty
 * @param QUEUE* q - The queue to use
 * @return -1 if the QUEUE is NULL, 1 if empty and 0 if not empty
 */
int is_queue_empty(QUEUE* q);

/**
 * Destroys the QUEUE and user data it contains
 * @param QUEUE* q - The queue to use
 * @return -1 if QUEUE is NULL, 0 on success
 */
int destroy_queue(QUEUE* q);

#endif
