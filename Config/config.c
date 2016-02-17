#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "config.h"
#include "link.h"

struct key_value_t {
    char type;
    char* key;
    void* value;
};

struct config_t {
    LIST* list;
};

void strip_newline(char* buf) {
    int i;
    for (i = 0; i < strlen(buf); i++) {
        if (buf[i] == '\n')
            buf[i] = '\0';
    }
}

struct key_value_t* process_int(char* buf) {
    struct key_value_t* node = NULL;
    char* node_key = NULL;
    int* node_value = NULL;
    char* type = NULL;
    char* name = NULL;
    char* value = NULL;

    node = malloc(sizeof(struct key_value_t));

    if (node == NULL) {
        return NULL;
    }

    type = strtok(buf, ":");
    name = strtok(NULL, "=");
    value = strtok(NULL, "");

    node_key = malloc(strlen(name) + 1);
    strcpy(node_key, name);

    node_value = malloc(sizeof(int));
    *node_value = atoi(value);

    node->key = node_key;
    node->value = node_value;
    node->type = 'I';

    return node;

}

struct key_value_t* process_double(char* buf) {
    struct key_value_t* node = NULL;
    char* node_key = NULL;
    double* node_value = NULL;
    char* type = NULL;
    char* name = NULL;
    char* value = NULL;

    node = malloc(sizeof(struct key_value_t));

    if (node == NULL) {
        return NULL;
    }

    type = strtok(buf, ":");
    name = strtok(NULL, "=");
    value = strtok(NULL, "");

    node_key = malloc(strlen(name) + 1);
    strcpy(node_key, name);

    node_value = malloc(sizeof(double));
    *node_value = atof(value);

    node->key = node_key;
    node->value = node_value;
    node->type = 'D';
    return node;

}

struct key_value_t* process_string(char* buf) {
    struct key_value_t* node = NULL;
    char* node_key = NULL;
    char* node_value = NULL;
    char* type = NULL;
    char* name = NULL;
    char* value = NULL;

    node = malloc(sizeof(struct key_value_t));

    if (node == NULL) {
        return NULL;
    }

    type = strtok(buf, ":");
    name = strtok(NULL, "=");
    value = strtok(NULL, "");

    node_key = malloc(strlen(name) + 1);
    strcpy(node_key, name);

    node_value = malloc(strlen(value) + 1);
    strcpy(node_value, value);

    node->key = node_key;
    node->value = node_value;
    node->type = 'S';

    return node;
}

struct key_value_t* process_boolean(char* buf) {
    struct key_value_t* node = NULL;
    char* node_key = NULL;
    char* node_value = NULL;
    char* type = NULL;
    char* name = NULL;
    char* value = NULL;

    node = malloc(sizeof(struct key_value_t));

    if (node == NULL) {
        return NULL;
    }

    type = strtok(buf, ":");
    name = strtok(NULL, "=");
    value = strtok(NULL, "");

    node_key = malloc(strlen(name) + 1);
    strcpy(node_key, name);

    node_value = malloc(sizeof(int));
    if (strcmp(value, "false") == 0)
        *node_value = FALSE;
    else
        *node_value = TRUE;

    node->key = node_key;
    node->value = node_value;
    node->type = 'B';
    return node;
}

void my_free(void* data) {
    struct key_value_t* d = NULL;
    d = (struct key_value_t*) data;
    free(d->key);
    free(d->value);
    free(d);
}

/* void* one is a string, while void* two */
/* is the struct */
int my_comp(void* one, void* two) {
    struct key_value_t* t = NULL;
    t = (struct key_value_t*) two;
    return strcmp((char*) one, t->key);
}

/**
 * Creates the config object to be used later
 * @return CONFIG* on success, NULL otherwise
 */
CONFIG* config_init() {
    CONFIG* c = NULL;
    LIST* l = NULL;
    c = malloc(sizeof(CONFIG));

    if (c == NULL) {
        return NULL;
    }

    l = list_init(my_free);
    c->list = l;

    return c;
}

/**
 * Loads a file's values into the config object
 * @param CONFIG* c - The config object to use
 * @param filename - File path and name to load
 * @return -1 in CONFIG is NULL, -2 if the file doesn't exist,
 *
 */
int config_load_file(CONFIG* c, char* filename) {
    FILE* file = NULL;
    char buf[1024];

    if (c == NULL) {
        return -1;
    }
    file = fopen(filename, "r");
    if (file == NULL) {
        return -2;
    }

    while (fgets(buf, 1024, file)) {
        if (buf[0] == '#' || buf[0] == '\n') {
            ;
        } else {
            strip_newline(buf);
            switch (buf[0]) {
                case 'I':
                    if (buf[1] == ':') {
                        list_add_tail(c->list, process_int(buf));
                    } else {
                        fprintf(stderr,
                                "Config Value - '%s'\nis an invalid value, Ignoring\n",
                                buf);
                    }
                    break;
                case 'D':
                    if (buf[1] == ':') {
                        list_add_tail(c->list, process_double(buf));
                    } else {
                        fprintf(stderr,
                                "Config Value - '%s'\nis an invalid value, Ignoring\n",
                                buf);
                    }
                    break;
                case 'S':
                    if (buf[1] == ':') {
                        list_add_tail(c->list, process_string(buf));
                    } else {
                        fprintf(stderr,
                                "Config Value - '%s'\nis an invalid value, Ignoring\n",
                                buf);
                    }
                    break;
                case 'B':
                    if (buf[1] == ':') {
                        list_add_tail(c->list, process_boolean(buf));
                    } else {
                        fprintf(stderr,
                                "Config Value - '%s'\nis an invalid value, Ignoring\n",
                                buf);
                    }
                    break;
                default:
                    fprintf(stderr,
                            "Config Value - '%s'\nis an invalid value, Ignoring\n",
                            buf);
            }
        }
    }
    fclose(file);
}

/**
 * frees the config and all data loaded
 * @param c - The config to use
 * @return -1 if c is NULL, -2 if data freeing failed
 * and 0 on success
 */
int config_destroy(CONFIG* c) {
    if (c == NULL) {
        return -1;
    }
    int re=list_destroy(c->list);
    if(re==0){
        free(c);
        return 0;
    }
    return -2;
}

/**
 * gets an int with the set key
 * @param c - The config to use
 * @param key - The key to look for
 * @return VALUE_NOT_FOUND if c is NULL or teh value is not found
 * the value otherwise
 */
int config_get_int(CONFIG* c, char* key) {
    struct key_value_t* node = NULL;
    int* re = NULL;

    if (c == NULL) {
        return VALUE_NOT_FOUND;
    }

    node = list_get(c->list, my_comp, key);
    if (node == NULL)
        return VALUE_NOT_FOUND;

    if (node->type != 'I')
        printf("Requested INT by '%s' is not type INT\n", key);
    re = (int*) node->value;
    return *re;
}

/**
 * gets an double with the set key
 * @param c - The config to use
 * @param key - The key to look for
 * @return VALUE_NOT_FOUND if c is NULL or teh value is not found
 * the value otherwise
 */
double config_get_double(CONFIG* c, char* key) {
    struct key_value_t* node = NULL;
    double* re = NULL;

    if (c == NULL) {
        return VALUE_NOT_FOUND;
    }

    node = list_get(c->list, my_comp, key);
    if (node == NULL)
        return VALUE_NOT_FOUND;

    if (node->type != 'D')
        printf("Requested DBL by '%s' is not type DBL\n", key);
    re = (double*) node->value;
    return *re;
}

/**
 * gets an string with the set key
 * @param c - The config to use
 * @param key - The key to look for
 * @return VALUE_NOT_FOUND if c is NULL or the value is not found
 * the value otherwise
 */
char* config_get_string(CONFIG* c, char* key) {
    struct key_value_t* node = NULL;
    char* re = NULL;

    if (c == NULL) {
        return VALUE_NOT_FOUND;
    }

    node = list_get(c->list, my_comp, key);
    if (node == NULL)
        return (char*) VALUE_NOT_FOUND;

    if (node->type != 'S')
        printf("Requested STRING by '%s' is not type STRING\n", key);
    re = (char*) node->value;
    return re;
}

/**
 * gets an boolean with the set key
 * @param c - The config to use
 * @param key - The key to look for
 * @return VALUE_NOT_FOUND if c is NULL or the value is not found
 * the value otherwise
 */
int config_get_boolean(CONFIG* c, char* key) {
    struct key_value_t* node = NULL;
    int re;

    if (c == NULL) {
        return VALUE_NOT_FOUND;
    }

    node = list_get(c->list, my_comp, key);
    if (node == NULL)
        return VALUE_NOT_FOUND;

    if (node->type != 'I')
        printf("Requested BOOL by '%s' is not type BOOL\n", key);
    re = *(int*) node->value;
    return re;
}
