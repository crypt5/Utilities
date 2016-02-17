#ifndef DATA_LOGGER_H_
#define DATA_LOGGER_H_

typedef struct data_logger_t DATA_LOGGER;

/**
 * Sets up a new data logger
 * @param filename - The filename to use for logging
 * @return DATA_LOGGER* on success, NULL on failure
 */
DATA_LOGGER *data_logger_init(char* filename);

/**
 * Adds the message to the logging queue
 * @param log - The data logger to use
 * @param message - The data to add to the log
 * @return 0 on success, -1 for message malloc fail
 * -2 for enqueue fail
 */
int data_logger_log(DATA_LOGGER *log, const char* message);

/**
 * Shuts down the logger and writes the remaining data to file
 * @param log - The logger to use
 * @return 0 on success, -1 if data freeing failed.
 */
int data_logger_shutdown(DATA_LOGGER *log);

#endif
