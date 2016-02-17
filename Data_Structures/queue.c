#include <stdio.h>
#include <stdlib.h>
#include "queue.h"

typedef void (*ufree)(void*);

struct queue_node_t {
    void* data;
    struct queue_node_t* next;
};

struct queue_t {
    int elements;
    ufree the_free;
    struct queue_node_t* head;
    struct queue_node_t* tail;
};

/**
 * Creates an empty Queue
 * @param myfree - function pointer to free user data
 * @return QUEUE* on success, NULL on failure
 */
QUEUE* init_queue(void (*myfree)(void*)) {
    QUEUE* q = NULL;

    q = malloc(sizeof(QUEUE));
    if (q == NULL) {
        return NULL;
    }

    q->elements = 0;
    q->the_free = myfree;
    q->head = NULL;
    q->tail = NULL;
    return q;
}

/**
 * Adds an element to the QUEUE
 * @param QUEUE* q - the Queue to use
 * @param void* data - The data to add
 * @return -1 if QUEUE is NULL, -2 if node malloc fails
 * 0 on success
 */
int enqueue(QUEUE* q, void* data) {
    struct queue_node_t* node = NULL;

    if (q == NULL) {
        return -1;
    }

    node = malloc(sizeof(struct queue_node_t));
    if (node == NULL) {
        return -2;
    }

    node->data = data;
    node->next = NULL;

    if (q->elements == 0) {
        q->tail = node;
        q->head = node;
    } else {
        q->tail->next = node;
        q->tail = node;
    }
    q->elements++;
    return 0;
}

/**
 * Removes an element from the queue and returns pointer
 * @param QUEUE* q - The queue to use
 * @return NULL if QUEUE is NULL or empty,
 * a pointer to the data otherwise
 */
void* dequeue(QUEUE* q) {
    void* temp = NULL;
    struct queue_node_t* hold = NULL;

    if (q == NULL || q->elements == 0) {
        return NULL;
    }

    hold = q->head;
    temp = q->head->data;
    q->head = q->head->next;

    free(hold);

    q->elements--;
    return temp;
}

/**
 * peek at the top element
 * @param QUEUE* q - The queue to use
 * @return NULL if the Queue is NULL or empty
 * a pointer to the data otherwise.
 */
void* peek_queue(QUEUE* q) {
    if (q == NULL || q->elements == 0) {
        return NULL;
    }

    return q->head->data;
}

/**
 * Function to see of the queue is empty
 * @param QUEUE* q - The queue to use
 * @return -1 if the QUEUE is NULL, 1 if empty and 0 if not empty
 */
int is_queue_empty(QUEUE* q) {
    if (q == NULL) {
        return -1;
    }

    if (q->elements == 0)
        return 1;
    else
        return 0;
}

/**
 * Destroys the QUEUE and user data it contains
 * @param QUEUE* q - The queue to use
 * @return -1 if QUEUE is NULL, 0 on success
 */
int destroy_queue(QUEUE* q) {

    if (q == NULL) {
        return -1;
    }

    while (is_queue_empty(q) == 0) {
        q->the_free(dequeue(q));
    }
    free(q);
    return 0;
}
