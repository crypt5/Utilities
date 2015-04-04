#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "config.h"
#include "link.h"

struct key_value_t{
  char type;
  char* key;
  void* value;
};

struct config_t{
  LIST* list;
};

void strip_newline(char* buf)
{
  int i;
  for(i=0;i<strlen(buf);i++){
    if(buf[i]=='\n')
      buf[i]='\0';
  }
}

struct key_value_t* process_int(char* buf)
{
  struct key_value_t* node=NULL;
  char* node_key=NULL;
  int* node_value=NULL;
  char* type=NULL;
  char* name=NULL;
  char* value=NULL;

  node=malloc(sizeof(struct key_value_t));

  if(node==NULL){
    printf("Key value node Malloc failed\n");
    exit(-1);
  }

  type=strtok(buf,":");
  name=strtok(NULL,"=");
  value=strtok(NULL,"");

  node_key=malloc(strlen(name)+1);
  strcpy(node_key,name);

  node_value=malloc(sizeof(int));
  *node_value=atoi(value);

  node->key=node_key;
  node->value=node_value;
  node->type='I';

  return node;

}

struct key_value_t* process_double(char* buf)
{
  struct key_value_t* node=NULL;
  char* node_key=NULL;
  double* node_value=NULL;
  char* type=NULL;
  char* name=NULL;
  char* value=NULL;

  node=malloc(sizeof(struct key_value_t));

  if(node==NULL){
    printf("Key value node Malloc failed\n");
    exit(-1);
  }

  type=strtok(buf,":");
  name=strtok(NULL,"=");
  value=strtok(NULL,"");

  node_key=malloc(strlen(name)+1);
  strcpy(node_key,name);

  node_value=malloc(sizeof(double));
  *node_value=atof(value);

  node->key=node_key;
  node->value=node_value;
  node->type='D';
  return node;

}

struct key_value_t* process_string(char* buf)
{
  struct key_value_t* node=NULL;
  char* node_key=NULL;
  char* node_value=NULL;
  char* type=NULL;
  char* name=NULL;
  char* value=NULL;

  node=malloc(sizeof(struct key_value_t));

  if(node==NULL){
    printf("Key value node Malloc failed\n");
    exit(-1);
  }

  type=strtok(buf,":");
  name=strtok(NULL,"=");
  value=strtok(NULL,"");

  node_key=malloc(strlen(name)+1);
  strcpy(node_key,name);

  node_value=malloc(strlen(value)+1);
  strcpy(node_value,value);

  node->key=node_key;
  node->value=node_value;
  node->type='S';

  return node;
}

struct key_value_t* process_boolean(char* buf)
{
  struct key_value_t* node=NULL;
  char* node_key=NULL;
  char* node_value=NULL;
  char* type=NULL;
  char* name=NULL;
  char* value=NULL;

  node=malloc(sizeof(struct key_value_t));

  if(node==NULL){
    printf("Key value node Malloc failed\n");
    exit(-1);
  }

  type=strtok(buf,":");
  name=strtok(NULL,"=");
  value=strtok(NULL,"");

  node_key=malloc(strlen(name)+1);
  strcpy(node_key,name);

  node_value=malloc(sizeof(int));
  if(strcmp(value,"false")==0)
    *node_value=FALSE;
  else
    *node_value=TRUE;

  node->key=node_key;
  node->value=node_value;
  node->type='B';
  return node;
}

void my_free(void* data)
{
  struct key_value_t* d=NULL;
  d=(struct key_value_t*)data;
  free(d->key);
  free(d->value);
  free(d);
}

/* void* one is a string, while void* two */
/* is the struct */
int my_comp(void* one, void* two)
{
  struct key_value_t* t=NULL;
  t=(struct key_value_t*)two;
  return strcmp((char*)one,t->key);
}

CONFIG* config_init()
{
  CONFIG* c=NULL;
  LIST* l=NULL;
  c=malloc(sizeof(CONFIG));

  if(c==NULL){
    printf("Config Malloc Failed!\n");
    exit(-1);
  }

  l=list_init(my_free,my_comp);
  c->list=l;

  return c;
}
void config_load_file(CONFIG* c,char* filename)
{
  FILE* file=NULL;
  char buf[1024];

  if(c==NULL){
    printf("Config Is NULL, unable to load\n");
    exit(-1);
  }
  file=fopen(filename,"r");
  if(file==NULL){
    printf("Invalid Config file - '%s'\tIgnoring file\n",filename);
    return;
  }

  while(fgets(buf,1024,file)){
    if(buf[0]=='#'||buf[0]=='\n')
      ;
    else{
      strip_newline(buf);
      switch(buf[0]){
      case 'I':
	if(buf[1]==':'){
	  list_add(c->list,process_int(buf));
	}
	else{
	  printf("Config Value - '%s'\nis an invalid value, Ignoring\n",buf);
	}
	break;
      case 'D':
	if(buf[1]==':'){
	  list_add(c->list,process_double(buf));
	}
	else{
	  printf("Config Value - '%s'\nis an invalid value, Ignoring\n",buf);
	}
	break;
      case 'S':
	if(buf[1]==':'){
	   list_add(c->list,process_string(buf));
	}
	else{
	  printf("Config Value - '%s'\nis an invalid value, Ignoring\n",buf);
	}
	break;
      case 'B':
	if(buf[1]==':'){
	  list_add(c->list,process_boolean(buf));
	}
	else{
	  printf("Config Value - '%s'\nis an invalid value, Ignoring\n",buf);
	}
	break;
      default:
	printf("Config Value - '%s'\nis an invalid value, Ignoring\n",buf);
      }
    }
  }
  fclose(file);
}
void config_destroy(CONFIG* c)
{
  if(c==NULL){
    printf("Config is NULL\n");
    exit(-1);
  }
  list_destroy(c->list);
  free(c);
}

int config_get_int(CONFIG* c, char* key)
{
  struct key_value_t* node=NULL;
  int* re=NULL;

  if(c==NULL){
    printf("Config is NULL, can't retrieve int\n");
    exit(-1);
  }

  node=list_get(c->list,key);
  if(node==NULL)
    return VALUE_NOT_FOUND;
  
  if(node->type!='I')
    printf("Requested INT by '%s' is not type INT\n",key);
  re=(int*)node->value;
  return *re;
}
double config_get_double(CONFIG* c, char* key)
{
  struct key_value_t* node=NULL;
  double* re=NULL;

  if(c==NULL){
    printf("Config is NULL, can't retrieve int\n");
    exit(-1);
  }

  node=list_get(c->list,key);
  if(node==NULL)
    return VALUE_NOT_FOUND;
  
  if(node->type!='D')
    printf("Requested DBL by '%s' is not type DBL\n",key);
  re=(double*)node->value;
  return *re;
}

char* config_get_string(CONFIG* c, char* key)
{
  struct key_value_t* node=NULL;
  char* re=NULL;

  if(c==NULL){
    printf("Config is NULL, can't retrieve int\n");
    exit(-1);
  }

  node=list_get(c->list,key);
  if(node==NULL)
    return (char*)VALUE_NOT_FOUND;
  
  if(node->type!='S')
    printf("Requested STRING by '%s' is not type STRING\n",key);
  re=(char*)node->value;
  return re;
}

int config_get_boolean(CONFIG* c, char* key)
{
  struct key_value_t* node=NULL;
  int re;

  if(c==NULL){
    printf("Config is NULL, can't retrieve int\n");
    exit(-1);
  }

  node=list_get(c->list,key);
  if(node==NULL)
    return VALUE_NOT_FOUND;

  if(node->type!='I')
    printf("Requested BOOL by '%s' is not type BOOL\n",key);
  re=*(int*)node->value;
  return re;
}
