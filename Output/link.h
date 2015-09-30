#ifndef LINK_H_
#define LINK_H_

typedef struct link_list_t LIST;

LIST* list_init(void(*ufree)(void*),int(*ucomp)(void*,void*));
void list_add_tail(LIST* l,void* data);
void list_add_head(LIST* l,void* data);
void* list_get(LIST* l,void* lookfor); /* Returns NULL if not found */
void* list_get_pos(LIST* l,int index);
void list_delete(LIST* l,void* lookfor);/* deletes object */
int list_length(LIST* l);
int is_list_empty(LIST* l);
void list_destroy(LIST* l);

void list_walk_reset(LIST* l);
void* list_get_next(LIST* l);

#endif
