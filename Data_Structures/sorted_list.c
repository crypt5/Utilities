#include <stdio.h>
#include <stdlib.h>
#include "sorted_list.h"

typedef void(*ufree)(void*);
typedef int(*ucomp)(void*,void*);

struct list_node_t{
  void* data;
  struct list_node_t* next;
  struct list_node_t* prev;
};

struct sorted_list_t{
  int elements;
  ufree the_free;
  ucomp the_comp;
  struct list_node_t* next;
  struct list_node_t* head;
  struct list_node_t* tail;
};


SORTED_LIST* sorted_list_init(void(*ufree)(void*),int(*ucomp)(void*,void*))
{
	SORTED_LIST* l=NULL;
	l=malloc(sizeof(SORTED_LIST));
	if(l==NULL){
		printf("Sorted List Malloc Failed\n");
		exit(-1);
	}
	l->elements=0;
	l->the_free=ufree;
	l->the_comp=ucomp;
	l->next=NULL;
	l->head=NULL;
	l->tail=NULL;
	return l;
}


void sorted_list_add(SORTED_LIST* l,void* data)
{
	if(l==NULL){
		printf("Sorted List is NULL\n");
		exit(-2);
	}
	struct list_node_t* node=malloc(sizeof(struct list_node_t));
	if(node==NULL){
		printf("Node Malloc Failed!\n");
		exit(-3);
	}
	node->next=NULL;
	node->prev=NULL;
	node->data=data;

	if(l->head==NULL){
		l->head=node;
		l->tail=node;
		l->elements++;
		return;
	}


	int i,comp;
	struct list_node_t* one=l->head;
	for(i=0;i<l->elements;i++){
		comp=l->the_comp(one->data,node->data);

		if(comp<0){
			if(one==l->head){
				l->head=node;
				one->prev=node;
				node->next=one;
				break;
			} else {
				one->prev->next=node;
				node->prev=one->prev;
				node->next=one;
				one->prev=node;
				break;
			}
		} else if(comp==0){
			if(one==l->head){
				l->head=node;
				one->prev=node;
				node->next=one;
				break;
			} else {
				one->prev->next=node;
				node->prev=one->prev;
				node->next=one;
				one->prev=node;
				break;
			}
		}

		if(one->next==NULL){
			node->prev=l->tail;
			l->tail->next=node;
			l->tail=node;
			break;
		}
		one=one->next;
	}
	l->elements++;
}

void* sorted_list_get(SORTED_LIST* l,void* lookfor) //Returns NULL if not found
{
	if(l==NULL){
		printf("List Data Structure is NULL! Can't get Element\n");
		exit(-4);
	}
	struct list_node_t* node=l->head;
	int comp;
	while(node!=NULL){
		comp=l->the_comp(node->data,lookfor);
		if(comp==0){
			return node->data;
		}
		node=node->next;
	}
	return NULL;
}

void sorted_list_delete(SORTED_LIST* l,void* lookfor)// deletes object
{
	if(l==NULL){
		printf("List Data Structure is NULL! Can't DeleteElement\n");
		exit(-5);
	}
	struct list_node_t* node=l->head;
	int comp;
	while(node!=NULL){
		comp=l->the_comp(node->data,lookfor);
		if(comp==0){
			if(node==l->head){
				l->head=node->next;
				node->next->prev=NULL;
			} else if(node==l->tail){
				l->tail=node->prev;
				node->prev->next=NULL;
			} else {
				node->prev->next=node->next;
				node->next->prev=node->prev;
			}
			l->the_free(node->data);
			free(node);
			l->elements--;
			break;
		}
		node=node->next;
	}
}


void* sorted_list_get_pos(SORTED_LIST* l,int index)
{
	if(l==NULL){
		printf("List Data Structure is NULL! Can't get Element\n");
		exit(-6);
	}
	struct list_node_t* node=l->head;
	int i,ele=l->elements;
	if(index<0||index>ele)
		return NULL;
	for(i=1;i<=index;i++){
		node=node->next;
	}
	return node->data;
}


int sorted_list_length(SORTED_LIST* l)
{
	if(l==NULL){
		printf("List Data Structure is NULL!\n");
		return -1;
	}
	return l->elements;
}

int is_sorted_list_empty(SORTED_LIST* l)
{
	if(l==NULL){
		printf("List Data Structure is NULL!\n");
		exit(-7);
	}
	if(l->elements==0)
		return 1;
	else
		return 0;
}

void sorted_list_destroy(SORTED_LIST* l)
{
	if(l==NULL){
		printf("List Data Structure is NULL!\n");
		exit(-8);
	}

	struct list_node_t* prev=l->head;
	if(prev==NULL){
		free(l);
		return;
	}
	struct list_node_t* node=l->head->next;

	while(node!=NULL){
		l->the_free(prev->data);
		free(prev);
		prev=node;
		node=node->next;
	}

	l->the_free(prev->data);
	free(prev);
	free(l);
}

void sorted_list_walk_reset(SORTED_LIST* l)
{
	if(l==NULL){
		printf("List Data Structure is NULL!\n");
		exit(-9);
	}
	l->next=l->head;
}

void* sorted_list_get_next(SORTED_LIST* l)
{
	if(l==NULL){
		printf("List Data Structure is NULL! Can't Get next\n");
		exit(-10);
	}
	struct list_node_t* re=l->next;
	if(re==NULL)
		return NULL;
	l->next=re->next;
	return re->data;
}
