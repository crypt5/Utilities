#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>
#include "link.h"

#define SIZE 100000

void my_free(void* data)
{
  free(data);
}

int my_comp(void* one, void* two)
{
  return *(int*)one-*(int*)two;
}

int main()
{
  LIST* list=list_init(my_free,my_comp);
  int i=0;
  int* data=NULL;
  time_t start,end;

  for(i=0;i<SIZE;i++){
    data=malloc(sizeof(int));
    *data=i;
    list_add_tail(list,data);
  }

  start=clock();
  for(i=0;i<SIZE;i++){
    data=list_get_pos(list,i);
    printf("%d\n",*data);
  }
  end=clock();
  printf("Old Time: %lf\n",(double)((end-start)/CLOCKS_PER_SEC));

  start=clock();
  list_walk_reset(list);
  while((data=list_get_next(list))!=NULL)
    printf("%d\n",*data);
  end=clock();
  printf("New Time: %lf\n",(double)((end-start)/CLOCKS_PER_SEC));

  list_destroy(list);
  return 0;
}
