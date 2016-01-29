#ifndef SORTED_LIST_H_
#define SORTED_LIST_H_

typedef struct sorted_list_t SORTED_LIST;

SORTED_LIST* sorted_list_init(void(*ufree)(void*),int(*ucomp)(void*,void*));
void sorted_list_add(SORTED_LIST* l,void* data);
void* sorted_list_get(SORTED_LIST* l,void* lookfor); /* Returns NULL if not found */
void* sorted_list_get_pos(SORTED_LIST* l,int index);
void sorted_list_delete(SORTED_LIST* l,void* lookfor);/* deletes object */
int sorted_list_length(SORTED_LIST* l);
int is_sorted_list_empty(SORTED_LIST* l);
void sorted_list_destroy(SORTED_LIST* l);

void sorted_list_walk_reset(SORTED_LIST* l);
void* sorted_list_get_next(SORTED_LIST* l);

#endif
