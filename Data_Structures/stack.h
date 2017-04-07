#ifndef STACK_H_
#define STACK_H_

typedef struct stack_t stack;

stack* create_stack();
int push_stack(stack* s, void* data);
void* pop_stack(stack* s);
int dispose_stack(stack* s,void(*ufree)(void*));

#endif /* STACK_H_ */
