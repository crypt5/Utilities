#ifndef CONFIG_H_
#define CONFIG_H_

#define TRUE 0
#define FALSE 1

#define VALUE_NOT_FOUND 52637489

typedef struct config_t CONFIG;

/**
 * Creates the config object to be used later
 * @return CONFIG* on success, NULL otherwise
 */
CONFIG* config_init();

/**
 * Loads a file's values into the config object
 * @param CONFIG* c - The config object to use
 * @param filename - File path and name to load
 * @return -1 in CONFIG is NULL, -2 if the file doesn't exist,
 *
 */
int config_load_file(CONFIG* c, char* filename);

/**
 * frees the config and all data loaded
 * @param c - The config to use
 * @return -1 if c is NULL, -2 if data freeing failed
 * and 0 on success
 */
int config_destroy(CONFIG* c);

/**
 * gets an int with the set key
 * @param c - The config to use
 * @param key - The key to look for
 * @return VALUE_NOT_FOUND if c is NULL or teh value is not found
 * the value otherwise
 */
int config_get_int(CONFIG* c, char* key);

/**
 * gets an double with the set key
 * @param c - The config to use
 * @param key - The key to look for
 * @return VALUE_NOT_FOUND if c is NULL or teh value is not found
 * the value otherwise
 */
double config_get_double(CONFIG* c, char* key);

/**
 * gets an string with the set key
 * @param c - The config to use
 * @param key - The key to look for
 * @return VALUE_NOT_FOUND if c is NULL or the value is not found
 * the value otherwise
 */
char* config_get_string(CONFIG* c, char* key);

/**
 * gets an boolean with the set key
 * @param c - The config to use
 * @param key - The key to look for
 * @return VALUE_NOT_FOUND if c is NULL or the value is not found
 * the value otherwise
 */
int config_get_boolean(CONFIG* c, char* key);

#endif
