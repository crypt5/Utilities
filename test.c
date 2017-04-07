#include <stdio.h>
#include <stdlib.h>
#include <kenutil/stack.h>

void my_free(void* data){
	free(data);
}

int main(int argc, char** argv) {
	int* one = malloc(sizeof(int));
	*one=5;

	stack* s = create_stack();

	push_stack(s,one);

	dispose_stack(s,my_free);

	return 0;
}
