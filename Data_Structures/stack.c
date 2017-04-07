#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include "stack.h"

struct element_t {
	struct element_t* prev;
	void* data;
};

struct stack_t {
	struct element_t* head;
};

stack* create_stack() {
	stack* s = malloc(sizeof(struct stack_t));
	if (s) {
		s->head = NULL;
		return s;
	}
	return NULL;
}

int push_stack(stack* s, void* data) {
	struct element_t* ele = malloc(sizeof(struct element_t));
	if (ele) {
		ele->data = data;
		if (s->head == NULL) {
			s->head = ele;
			ele->prev = NULL;
		} else {
			ele->prev = s->head;
			s->head = ele;
		}
		return 0;
	}
	return -1;
}

void* pop_stack(stack* s) {
	if (s->head != NULL) {
		struct element_t* ele = s->head;
		s->head = ele->prev;
		void* data = ele->data;
		free(ele);
		return data;
	}
	return NULL;
}

int dispose_stack(stack* s, void (*ufree)(void*)) {
	void* data;
	while ((data = pop_stack(s)) != NULL) {
		ufree(data);
	}
	free(s);
	return 1;
}
