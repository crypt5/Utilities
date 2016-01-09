#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>
#include <string.h>
#include <time.h>
#include "data_logger.h"
#include "queue.h"

struct data_logger_t{
  pthread_mutex_t lock;
  pthread_t thread;
  QUEUE* lines;
  FILE* log;
  char run;
};

/* Needed by queue library */
void my_free(void* data) 
{
  free(data);
}

void* loop(void* data)
{
  DATA_LOGGER* log=NULL;
  char* temp=NULL;
  char runner=1;

  log=(DATA_LOGGER*)data;

  while(runner){
    pthread_mutex_lock(&log->lock);
    runner=log->run;
    if(is_queue_empty(log->lines)!=1){
      temp=(char*)dequeue(log->lines);
      fprintf(log->log,"%s\n",temp);
      free(temp);
    }
    
    if(runner==0){
      while(is_queue_empty(log->lines)!=1){
	temp=(char*)dequeue(log->lines);
	fprintf(log->log,"%s\n",temp);
	free(temp);
      }
    }

    pthread_mutex_unlock(&log->lock);
    /* one milli */
    usleep(1000);
  }

  return NULL;
}

DATA_LOGGER *data_logger_init(char* filename)
{
  DATA_LOGGER* log=NULL;

  log=malloc(sizeof(DATA_LOGGER));
  if(log==NULL){
    printf("Logger malloc failed\n");
    exit(-1);
  }

  log->log=fopen(filename,"w");
  log->lines=init_queue(my_free);
  log->run=1;

  if(pthread_mutex_init(&log->lock,NULL)==1){
    printf("Mutex creation Failed\n");
    exit(-1);
  }

  if(pthread_create(&log->thread,NULL, &loop, log)==1){
    printf("Thread Creation failed\n");
    exit(-1);
  }

  return log;
}

void data_logger_log(DATA_LOGGER *log,char* message)
{
  char *sub;
  sub=malloc(strlen(message)+1);
  strcpy(sub,message);
  pthread_mutex_lock(&log->lock);
  enqueue(log->lines,sub);
  pthread_mutex_unlock(&log->lock);
}

void data_logger_shutdown(DATA_LOGGER *log)
{
  pthread_mutex_lock(&log->lock);
  log->run=0;
  pthread_mutex_unlock(&log->lock);
  pthread_join(log->thread,NULL);

  fclose(log->log);
  pthread_mutex_destroy(&log->lock);
  destroy_queue(log->lines);

  free(log);
}
