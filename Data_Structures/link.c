#include <stdio.h>
#include <stdlib.h>
#include "link.h"

typedef void (*ufree)(void*);

struct list_node_t {
    void* data;
    struct list_node_t* next;
    struct list_node_t* prev;
};

struct link_list_t {
    int elements;
    ufree the_free;
    struct list_node_t* next;
    struct list_node_t* head;
    struct list_node_t* tail;
};

/**
 * This functions creates an empty linked list
 * @param void ufree(void*) - Function pointer to the function to free the data
 * @return LIST* if everything worked, NULL otherwise.
 */
LIST* list_init(void (*ufree)(void*)) {
    LIST* list = NULL;
    list = malloc(sizeof(struct link_list_t));
    if (list == NULL) {
        return NULL;
    }
    list->elements = 0;
    list->head = NULL;
    list->tail = NULL;
    list->next = NULL;
    list->the_free = ufree;

    return list;
}

/**
 * Adds and element to the tail of the list
 * @param LIST* l - The list to add to
 * @param void* data - The data to add to the list
 * @return -1 if LIST is NULL, -2 if Node malloc fail, 0 on success
 */
int list_add_tail(LIST* l, void* data) {
    struct list_node_t* node = NULL;

    if (l == NULL) {
        return -1;
    }

    node = malloc(sizeof(struct list_node_t));

    if (node == NULL) {
        return -2;
    }

    node->next = NULL;
    node->data = data;

    if (l->elements == 0) {
        l->head = node;
        l->tail = node;
    } else {
        l->tail->next = node;
        node->prev = l->tail;
        l->tail = node;
    }

    l->elements++;
    return 0;
}

/**
 * Adds an element to the head of the list
 * @param LIST* l - The list to add to
 * @param void* data - The data to add
 * @return -1 if LIST is NULL, -2 if node malloc fail, 0 on success
 */
int list_add_head(LIST* l, void* data) {
    struct list_node_t* node = NULL;

    if (l == NULL) {
        return -1;
    }

    node = malloc(sizeof(struct list_node_t));

    if (node == NULL) {
        return -2;
    }

    node->next = NULL;
    node->data = data;

    if (l->elements == 0) {
        l->head = node;
        l->tail = node;
    } else {
        node->next = l->head;
        l->head = node;
    }

    l->elements++;
    return 0;
}

/**
 * Function to access an element in the list
 * @param LIST* l - The list to search
 * @param ucomp - Compare function, should return -1,0,1 like strcmp
 * @param void* lookfor - pointer to the data to look for
 * @return NULL if LIST is NULL, or the element is not found.
 *  A pointer to the element on success
 */
void* list_get(LIST* l, int (*ucomp)(void*, void*), void* lookfor) {
    struct list_node_t* temp = NULL;
    int comp;
    int i = 0;

    if (l == NULL) {
        return NULL;
    }

    if (l->elements != 0) {
        temp = l->head;
        while (temp != NULL && i < l->elements) {
            comp = ucomp(lookfor, temp->data);
            if (comp == 0)
                return temp->data;
            temp = temp->next;
            i++;
        }
        return NULL;
    } else {
        return NULL;
    }

}

/**
 * Removes an item from the list
 * @param LIST* l - The list to use
 * @param ucomp - User defined compare function
 * @param void* lookfor - Data to search for
 * @return -1 if LIST is NULL, 0 on success
 */
int list_delete(LIST* l, int (*ucomp)(void*, void*), void* lookfor) {
    struct list_node_t* temp = NULL;
    struct list_node_t* prev = NULL;
    int i = 0;
    int comp;

    if (l == NULL) {
        return -1;
    }

    if (l->elements == 1) {
        temp = l->head;
        l->head = NULL;
        l->tail = NULL;
        l->elements--;
        l->the_free(temp->data);
        free(temp);
    } else if (l->elements > 1) {
        temp = l->head;
        while (temp != NULL && i < l->elements) {
            comp = ucomp(lookfor, temp->data);
            if (comp == 0) {
                if (temp == l->head) {
                    l->head = temp->next;
                    ;
                    l->the_free(temp->data);
                    free(temp);
                    l->elements--;
                    return 0;
                } else if (temp == l->tail) {
                    l->tail = prev;
                    prev->next = NULL;
                    l->the_free(temp->data);
                    free(temp);
                    l->elements--;
                    return 0;
                } else {
                    prev->next = temp->next;
                    l->the_free(temp->data);
                    free(temp);
                    l->elements--;
                    return 0;
                }
            }
            prev = temp;
            temp = temp->next;
            i++;
        }
    } else {
        /* Do nothing - list is empty */
    }
    return 0;
}

/**
 * Boolean for is the list empty
 * @param LIST* l - Pointer to the list element
 * @return -1 if LIST is NULL, 0 if not empty, and 1 if empty
 */
int is_list_empty(LIST* l) {
    if (l == NULL) {
        return -1;
    }

    if (l->elements == 0)
        return 1;
    else
        return 0;
}

/**
 * Function to get the list's length
 * @param LIST* l - the list to use
 * @return -1 if LIST is NULL, the length otherwise
 */
int list_length(LIST* l) {
    if (l == NULL) {
        return -1;
    }

    return l->elements;
}

/**
 * Function to clean up the list and user data contained
 * @param LIST* l - The list to free
 * @return -1 if LIST is NULL, 0 on success
 */
int list_destroy(LIST* l) {
    struct list_node_t* next = NULL;
    struct list_node_t* cur = NULL;

    if (l == NULL) {
        return -1;
    }

    if (l->elements == 0) {
        free(l);
    } else if (l->elements == 1) {
        cur = l->head;
        l->the_free(cur->data);
        free(cur);
        free(l);
    } else {
        next = l->head->next;
        cur = l->head;
        while (next != NULL) {
            l->the_free(cur->data);
            free(cur);
            cur = next;
            next = next->next;
        }
        l->the_free(cur->data);
        free(cur);
        free(l);
    }
    return 0;
}

/**
 * Function to get data at a certian position in the list,
 * Lists start at index 0;
 * @param LIST* l - List to use
 * @param index - index to get the value of
 * @return NULL if LIST is NULL, or index > number of elements
 * a pointer to the data otherwise.
 */
void* list_get_pos(LIST* l, unsigned int index) {
    int i;
    struct list_node_t* temp = NULL;

    if (l == NULL) {
        return NULL;
    }
    if (index >= l->elements) {
        return NULL;
    }
    temp = l->head;
    for (i = 1; i <= index; i++) {
        temp = temp->next;
    }
    return temp->data;
}

/**
 * Makes the list ready to walk
 * @param LIST* l - List to reset
 * @return -1 if LIST is NULL, 0 on success
 */
int list_walk_reset(LIST* l) {
    if (l == NULL) {
        return -1;
    }
    l->next = l->head;
    return 0;
}

/**
 * Walks the list to take advantage of O(1) iteration
 * @param LIST* l - List to walk
 * @return NULL if LIST is empty or the end of the list,
 * a pointer to the data otherwise
 */
void* list_get_next(LIST* l) {
    struct list_node_t* temp = NULL;

    if (l == NULL) {
        return NULL;
    }
    temp = l->next;
    if (temp == NULL)
        return NULL;
    l->next = temp->next;
    return temp->data;
}
