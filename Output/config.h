#ifndef CONFIG_H_
#define CONFIG_H_

#define TRUE 0
#define FALSE 1

#define VALUE_NOT_FOUND 52637489

typedef struct config_t CONFIG;

CONFIG* config_init();
void config_load_file(CONFIG* c,char* filename);
void config_destroy(CONFIG* c); 

int config_get_int(CONFIG* c, char* key); 
double config_get_double(CONFIG* c, char* key);
char* config_get_string(CONFIG* c, char* key);
int config_get_boolean(CONFIG* c, char* key);


#endif
