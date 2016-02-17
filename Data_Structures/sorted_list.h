#ifndef SORTED_LIST_H_
#define SORTED_LIST_H_

typedef struct sorted_list_t SORTED_LIST;

/**
 * Creates an empty sorted list
 * @param ufree - Function pointer to user defined free function
 * @param ucomp - function pointer to the sorting function
 * @return SORTED_LIST* on success, NULL on failure
 */
SORTED_LIST* sorted_list_init(void (*ufree)(void*), int (*ucomp)(void*, void*));

/**
 * Adds an element to the sorted list while
 * maintaining the sorted order
 * @param SORTED_LIST* l - The list to use
 * @param void* data - The data to add the the list
 * @return -1 if LIST is NULL, -2 if node malloc fails,
 * 0 on success
 */
int sorted_list_add(SORTED_LIST* l, void* data);

/**
 * looks for the data in the list
 * @param LIST* l - The list to search in
 * @param void* lookfor - A pointer to the data to search for
 * @returns NULL if LIST is NULL or lookfor is not found,
 * a pointer to the found data otherwise
 */
void* sorted_list_get(SORTED_LIST* l, void* lookfor);

/**
 * Gets the data at the desired position, indexing starts at 0
 * @param LIST* l -  The list to use
 * @param index - The index to get the element of
 * @return NULL if the LIST is NULL or index > number of elements,
 * a pointer to the data at the index position otherwise
 */
void* sorted_list_get_pos(SORTED_LIST* l, int index);

/**
 * Deletes an element for the sorted list
 * @param LIST* l - The sorted list to use
 * @param void* lookfor - a pointer to the data to search for
 * @return -1 if LIST is NULL, 0 if element is deleted,
 * 1 if the element is not found
 */
void sorted_list_delete(SORTED_LIST* l, void* lookfor);

/**
 * Gets the length of the list
 * @param LIST* l - List to get the length of
 * @returns -1 if LIST is NULL, the length otherwise
 */
int sorted_list_length(SORTED_LIST* l);

/**
 * Function to check to see if the list is empty
 * @param LIST* l - List to use
 * @return -1 if LIST is NULL, 1 if empty, and 0 if not empty
 */
int is_sorted_list_empty(SORTED_LIST* l);

/**
 * Frees all memory used by the list and user data
 * @param LIST* l - List and data to free
 * @return -1 if LIST is NULL, 0 on success
 */
int sorted_list_destroy(SORTED_LIST* l);

/**
 * Resets the list to be walked using O(1)
 * @param LIST* l - The list to reset
 * @return -1 if LIST is NULL, 0 on success
 */
int sorted_list_walk_reset(SORTED_LIST* l);

/**
 * Gets the next element in the list
 * @param LIST* l -The list to use
 * @return NULL if the LIST is NULL, or the end has been reached
 * a pointer to the element otherwise
 */
void* sorted_list_get_next(SORTED_LIST* l);

#endif
