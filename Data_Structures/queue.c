#include <stdio.h>
#include <stdlib.h>
#include "queue.h"

typedef void(*ufree)(void*);

struct queue_node_t{
  void* data;
  struct queue_node_t* next;
};

struct queue_t{
  int elements;
  ufree the_free;
  struct queue_node_t* head;
  struct queue_node_t* tail;
};

QUEUE* init_queue(void(*myfree)(void*))
{
  QUEUE* q=NULL;

  q=malloc(sizeof(QUEUE));
  if(q==NULL){
    printf("Queue malloc failed!");
    exit(-1);
  }

  q->elements=0;
  q->the_free=myfree;
  q->head=NULL;
  q->tail=NULL;
  return q;
}

void enqueue(QUEUE* q,void* data)
{
  struct queue_node_t* node=NULL;

  if(q==NULL){
    printf("Can't enqueue, queue is NULL!\n");
    exit(-1);
  }

  node=malloc(sizeof(struct queue_node_t));
  if(node==NULL){
    printf("Node malloc failed!\n");
    exit(-1);
  }

  node->data=data;
  node->next=NULL;

  if(q->elements==0){
    q->tail=node;
    q->head=node;
  }
  else{
    q->tail->next=node;
    q->tail=node;
  }
  q->elements++;

}

void* dequeue(QUEUE* q)
{
  void* temp=NULL;
  struct queue_node_t* hold=NULL;

  if(q==NULL){
    printf("Can't Dequeue, Queue is NULL\n");
    exit(-1);
  }

  if(q->elements==0){
    printf("Queue is Empty! Can't Dequeue!\n");
    exit(-1);
  }

  hold=q->head;
  temp=q->head->data;
  q->head=q->head->next;

  free(hold);

  q->elements--;
  return temp;
}

void* peek_queue(QUEUE* q)
{
  if(q==NULL){
    printf("Queue NULL!\n");
    exit(-1);
  }

  if(q->elements==0)
    return NULL;

  return q->head->data;
}

int is_queue_empty(QUEUE* q)
{
  if(q==NULL){
    printf("QUEUE Empty!\n");
    exit(-1);
  }

  if(q->elements==0)
    return 1;
  else
    return 0;
}

void destroy_queue(QUEUE* q)
{

  if(q==NULL){
    printf("Queue is NULL!\n");
    exit(-1);
  }

  while(is_queue_empty(q)==0){
    q->the_free(dequeue(q));
  }
  free(q);
}
