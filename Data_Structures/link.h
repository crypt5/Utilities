#ifndef LINK_H_
#define LINK_H_

typedef struct link_list_t LIST;

/**
 * This functions creates an empty linked list
 * @param void ufree(void*) - Function pointer to the function to free the data
 * @return LIST* if everything worked, NULL otherwise.
 */
LIST* list_init(void (*ufree)(void*));

/**
 * Adds and element to the tail of the list
 * @param LIST* l - The list to add to
 * @param void* data - The data to add to the list
 * @return -1 if LIST is NULL, -2 if Node malloc fail, 0 on success
 */
int list_add_tail(LIST* l, void* data);

/**
 * Adds an element to the head of the list
 * @param LIST* l - The list to add to
 * @param void* data - The data to add
 * @return -1 if LIST is NULL, -2 if node malloc fail, 0 on success
 */
int list_add_head(LIST* l, void* data);

/**
 * Function to access an element in the list
 * @param LIST* l - The list to search
 * @param ucomp - Compare function, should return -1,0,1 like strcmp
 * @param void* lookfor - pointer to the data to look for
 * @return NULL if LIST is NULL, or the element is not found.
 *  A pointer to the element on success
 */
void* list_get(LIST* l, int (*ucomp)(void*, void*), void* lookfor);

/**
 * Function to get data at a certian position in the list,
 * Lists start at index 0;
 * @param LIST* l - List to use
 * @param index - index to get the value of
 * @return NULL if LIST is NULL, or index > number of elements
 * a pointer to the data otherwise.
 */
void* list_get_pos(LIST* l, unsigned int index);

/**
 * Removes an item from the list
 * @param LIST* l - The list to use
 * @param ucomp - User defined compare function
 * @param void* lookfor - Data to search for
 * @return -1 if LIST is NULL, 0 on success
 */
int list_delete(LIST* l, int (*ucomp)(void*, void*), void* lookfor);

/**
 * Function to get the list's length
 * @param LIST* l - the list to use
 * @return -1 if LIST is NULL, the length otherwise
 */
int list_length(LIST* l);

/**
 * Boolean for is the list empty
 * @param LIST* l - Pointer to the list element
 * @return -1 if LIST is NULL, 0 if not empty, and 1 if empty
 */
int is_list_empty(LIST* l);

/**
 * Function to clean up the list and user data contained
 * @param LIST* l - The list to free
 * @return -1 if LIST is NULL, 0 on success
 */
int list_destroy(LIST* l);

/**
 * Makes the list ready to walk
 * @param LIST* l - List to reset
 * @return -1 if LIST is NULL, 0 on success
 */
int list_walk_reset(LIST* l);

/**
 * Walks the list to take advantage of O(1) iteration
 * @param LIST* l - List to walk
 * @return NULL if LIST is empty or the end of the list,
 * a pointer to the data otherwise
 */
void* list_get_next(LIST* l);

#endif
