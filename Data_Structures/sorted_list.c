#include <stdio.h>
#include <stdlib.h>
#include "sorted_list.h"

typedef void (*ufree)(void*);
typedef int (*ucomp)(void*, void*);

struct list_node_t {
    void* data;
    struct list_node_t* next;
    struct list_node_t* prev;
};

struct sorted_list_t {
    int elements;
    ufree the_free;
    ucomp the_comp;
    struct list_node_t* next;
    struct list_node_t* head;
    struct list_node_t* tail;
};

/**
 * Creates an empty sorted list
 * @param ufree - Function pointer to user defined free function
 * @param ucomp - function pointer to the sorting function
 * @return SORTED_LIST* on success, NULL on failure
 */
SORTED_LIST* sorted_list_init(void (*ufree)(void*), int (*ucomp)(void*, void*)) {
    SORTED_LIST* l = NULL;
    l = malloc(sizeof(SORTED_LIST));
    if (l == NULL) {
        return NULL;
    }
    l->elements = 0;
    l->the_free = ufree;
    l->the_comp = ucomp;
    l->next = NULL;
    l->head = NULL;
    l->tail = NULL;
    return l;
}

/**
 * Adds an element to the sorted list while
 * maintaining the sorted order
 * @param SORTED_LIST* l - The list to use
 * @param void* data - The data to add the the list
 * @return -1 if LIST is NULL, -2 if node malloc fails,
 * 0 on success
 */
int sorted_list_add(SORTED_LIST* l, void* data) {
    if (l == NULL) {
        return -1;
    }
    struct list_node_t* node = malloc(sizeof(struct list_node_t));
    if (node == NULL) {
        return -2;
    }
    node->next = NULL;
    node->prev = NULL;
    node->data = data;

    if (l->head == NULL) {
        l->head = node;
        l->tail = node;
        l->elements++;
        return 0;
    }

    int i, comp;
    struct list_node_t* one = l->head;
    for (i = 0; i < l->elements; i++) {
        comp = l->the_comp(one->data, node->data);

        if (comp < 0) {
            if (one == l->head) {
                l->head = node;
                one->prev = node;
                node->next = one;
                break;
            } else {
                one->prev->next = node;
                node->prev = one->prev;
                node->next = one;
                one->prev = node;
                break;
            }
        } else if (comp == 0) {
            if (one == l->head) {
                l->head = node;
                one->prev = node;
                node->next = one;
                break;
            } else {
                one->prev->next = node;
                node->prev = one->prev;
                node->next = one;
                one->prev = node;
                break;
            }
        }

        if (one->next == NULL) {
            node->prev = l->tail;
            l->tail->next = node;
            l->tail = node;
            break;
        }
        one = one->next;
    }
    l->elements++;
    return 0;
}

/**
 * looks for the data in the list
 * @param LIST* l - The list to search in
 * @param void* lookfor - A pointer to the data to search for
 * @returns NULL if LIST is NULL or lookfor is not found,
 * a pointer to the found data otherwise
 */
void* sorted_list_get(SORTED_LIST* l, void* lookfor) {
    if (l == NULL) {
        return NULL;
    }
    struct list_node_t* node = l->head;
    int comp;
    while (node != NULL) {
        comp = l->the_comp(node->data, lookfor);
        if (comp == 0) {
            return node->data;
        }
        node = node->next;
    }
    return NULL;
}

/**
 * Deletes an element for the sorted list
 * @param LIST* l - The sorted list to use
 * @param void* lookfor - a pointer to the data to search for
 * @return -1 if LIST is NULL, 0 if element is deleted,
 * 1 if the element is not found
 */
int sorted_list_delete(SORTED_LIST* l, void* lookfor) {
    if (l == NULL) {
        return -1;
    }
    struct list_node_t* node = l->head;
    int comp;
    while (node != NULL) {
        comp = l->the_comp(node->data, lookfor);
        if (comp == 0) {
            if (node == l->head) {
                l->head = node->next;
                node->next->prev = NULL;
            } else if (node == l->tail) {
                l->tail = node->prev;
                node->prev->next = NULL;
            } else {
                node->prev->next = node->next;
                node->next->prev = node->prev;
            }
            l->the_free(node->data);
            free(node);
            l->elements--;
            return 0;
        }
        node = node->next;
    }
    return 1;
}

/**
 * Gets the data at the desired position, indexing starts at 0
 * @param LIST* l -  The list to use
 * @param index - The index to get the element of
 * @return NULL if the LIST is NULL or index > number of elements,
 * a pointer to the data at the index position otherwise
 */
void* sorted_list_get_pos(SORTED_LIST* l, unsigned int index) {
    if (l == NULL || index > l->elements) {
        return NULL;
    }
    struct list_node_t* node = l->head;
    int i;
    for (i = 1; i <= index; i++) {
        node = node->next;
    }
    return node->data;
}

/**
 * Gets the length of the list
 * @param LIST* l - List to get the length of
 * @returns -1 if LIST is NULL, the length otherwise
 */
int sorted_list_length(SORTED_LIST* l) {
    if (l == NULL) {
        return -1;
    }
    return l->elements;
}

/**
 * Function to check to see if the list is empty
 * @param LIST* l - List to use
 * @return -1 if LIST is NULL, 1 if empty, and 0 if not empty
 */
int is_sorted_list_empty(SORTED_LIST* l) {
    if (l == NULL) {
        return -1;
    }
    if (l->elements == 0)
        return 1;
    else
        return 0;
}

/**
 * Frees all memory used by the list and user data
 * @param LIST* l - List and data to free
 * @return -1 if LIST is NULL, 0 on success
 */
int sorted_list_destroy(SORTED_LIST* l) {
    if (l == NULL) {
        return -1;
    }

    struct list_node_t* prev = l->head;
    if (prev == NULL) {
        free(l);
        return 0;
    }
    struct list_node_t* node = l->head->next;

    while (node != NULL) {
        l->the_free(prev->data);
        free(prev);
        prev = node;
        node = node->next;
    }

    l->the_free(prev->data);
    free(prev);
    free(l);
    return 0;
}

/**
 * Resets the list to be walked using O(1)
 * @param LIST* l - The list to reset
 * @return -1 if LIST is NULL, 0 on success
 */
int sorted_list_walk_reset(SORTED_LIST* l) {
    if (l == NULL) {
        return -1;
    }
    l->next = l->head;
    return 0;
}

/**
 * Gets the next element in the list
 * @param LIST* l -The list to use
 * @return NULL if the LIST is NULL, or the end has been reached
 * a pointer to the element otherwise
 */
void* sorted_list_get_next(SORTED_LIST* l) {
    if (l == NULL) {
        return NULL;
    }
    struct list_node_t* re = l->next;
    if (re == NULL)
        return NULL;
    l->next = re->next;
    return re->data;
}
