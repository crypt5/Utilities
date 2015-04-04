#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>
#include <string.h>
#include <time.h>
#include "logger.h"
#include "queue.h"

struct logger_t{
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
  LOGGER* log=NULL;
  char* temp=NULL;
  char runner=1;

  log=(LOGGER*)data;

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
    /* Micro * milli */
    usleep(1000*100);
  }

  return NULL;
}

LOGGER *logger_init(char* filename)
{
  LOGGER* log=NULL;

  log=malloc(sizeof(LOGGER));
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

void logger_log(LOGGER *log,char* message)
{
  char* sub=NULL;
  int i;
  char time_out[1024];
  time_t rawtime;

  time(&rawtime);
  sprintf(time_out,"%s",ctime(&rawtime));
  time_out[strlen(time_out)-1]='\0';

  if(log==NULL){
    printf("Logger is NULL!!\n");
    exit(-1);
  }

  sub=malloc(strlen(time_out)+strlen(message)+4);
  if(sub==NULL){
    printf("String setup Failed\n");
    exit(-1);
  }
  sub[0]='[';
  for(i=0;i<strlen(time_out);i++)
    sub[i+1]=time_out[i];
  sub[strlen(time_out)+1]=']';
  sub[strlen(time_out)+2]=' ';
  sub[strlen(time_out)+3]='\0'; /* For str cat to work */
  strcat(sub,message);

  pthread_mutex_lock(&log->lock);
  enqueue(log->lines,sub);
  pthread_mutex_unlock(&log->lock);
}

void logger_shutdown(LOGGER *log)
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
