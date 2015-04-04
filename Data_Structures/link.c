#include <stdio.h>
#include <stdlib.h>
#include "link.h"

typedef void(*ufree)(void*);
typedef int(*ucomp)(void*,void*);

struct list_node_t{
  void* data;
  struct list_node_t* next;
  struct list_node_t* prev;
};

struct link_list_t{
  int elements;
  ufree the_free;
  ucomp the_comp;
  struct list_node_t* head;
  struct list_node_t* tail;
};

LIST* list_init(void(*ufree)(void*),int(*ucomp)(void*,void*))
{
  LIST* list=NULL;
  list=malloc(sizeof(struct link_list_t));
  if(list==NULL){
    printf("List Malloc Failed\n");
    exit(-1);
  }
  list->elements=0;
  list->head=NULL;
  list->the_free=ufree;
  list->the_comp=ucomp;

  return list;
}

void list_add(LIST* l,void* data)
{
  struct list_node_t* node=NULL;

  if(l==NULL){
    printf("Can't add to a NULL list\n");
    exit(-1);
  }

  node=malloc(sizeof(struct list_node_t));

  if(node==NULL){
    printf("Node Malloc failed\n");
    exit(-1);
  }

  node->next=NULL;
  node->data=data;

  if(l->elements==0){
    l->head=node;
    l->tail=node;
  }
  else{
    l->tail->next=node;
    node->prev=l->tail;
    l->tail=node;
  }

  l->elements++;

}

void* list_get(LIST* l,void* lookfor)
{
  struct list_node_t* temp=NULL;
  int comp;
  int i=0;

  if(l==NULL){
    printf("List is NULL!\n");\
    exit(-1);
  }

  if(l->elements!=0){
    temp=l->head;
    while(temp!=NULL&&i<l->elements){
      comp=l->the_comp(lookfor,temp->data);
      if(comp==0)
	return temp->data;
      temp=temp->next;
      i++;
    }
    return NULL;
  }
  else{
    return NULL;
  }

}

void list_delete(LIST* l,void* lookfor)
{
  struct list_node_t* temp=NULL;
  struct list_node_t* prev=NULL;
  int i=0;
  int comp;

  if(l==NULL){
    printf("List is NULL, nothing to delete\n");
    exit(-1);
  }

  if(l->elements==1){
    temp=l->head;
    l->head=NULL;
    l->tail=NULL;
    l->elements--;
    l->the_free(temp->data);
    free(temp);
  }
  else if(l->elements>1){
    temp=l->head;
    while(temp!=NULL&&i<l->elements){
      comp=l->the_comp(lookfor,temp->data);
      if(comp==0){
	if(temp==l->head){
	  l->head=temp->next;;
	  l->the_free(temp->data);
	  free(temp);
	  l->elements--;
	  break;
	}
	else if(temp==l->tail){
	  l->tail=prev;
	  prev->next=NULL;
	  l->the_free(temp->data);
	  free(temp);
	  l->elements--;
	  break;
	}
	else{
	  prev->next=temp->next;
	  l->the_free(temp->data);
	  free(temp);
	  l->elements--;
	  break;
	}
      }
      prev=temp;
      temp=temp->next;
      i++;
    }
  }
  else{
    /* Do nothing - list is empty */
  }

}

int is_list_empty(LIST* l)
{
  if(l==NULL){
    printf("List is NULL, No length, EMPTY\n");
    exit(-1);
  }

  if(l->elements==0)
    return 1;
  else
    return 0;
}

int list_length(LIST* l)
{
  if(l==NULL){
    printf("List is NULL, No length\n");
    exit(-1);
  }

  return l->elements;
}

void list_destroy(LIST* l)
{
  struct list_node_t* next=NULL;
  struct list_node_t* cur=NULL;

  if(l==NULL){
    printf("Can't Destroy, List is NULL\n");
    exit(-1);
  }

  if(l->elements==0){
    free(l);
  }
  else if(l->elements==1){
    cur=l->head;
    l->the_free(cur->data);
    free(cur);
    free(l);
  }
  else{
    next=l->head->next;
    cur=l->head;
    while(next!=NULL){
      l->the_free(cur->data);
      free(cur);
      cur=next;
      next=next->next;
    }
    l->the_free(cur->data);
    free(cur);
    free(l);
  }
}

void* list_get_pos(LIST* l,int index)
{
  int i;
  struct list_node_t* temp=NULL;

  if(l==NULL){
    printf("List is NULL, can't search\n");
    exit(-1);
  }
  if(index>l->elements)
    return NULL;
 
  temp=l->head;
  for(i=1;i<=index;i++){
    temp=temp->next;
  }
  return temp->data;
}
