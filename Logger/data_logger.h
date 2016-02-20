#ifndef DATA_LOGGER_H_
#define DATA_LOGGER_H_

typedef struct data_logger_t DATA_LOGGER;

/**
 * This created the data structures needed to use the data
 * logging library. This library is thread safe and double
 * buffered.
 * @param filename - The filename and path of the output
 * @param buffer_size - The size of the buffers in bytes.
 * pass a 0 to use the default 4096 size.
 * @param update_int - The time the file writing loop should
 * wait in Microseconds.
 * @return NULL if any malloc failed, A pointer to the LOGGER
 * otherwise
 */
DATA_LOGGER *data_logger_init(const char* filename, unsigned int buffer_size,
        unsigned int update_int);

/**
 * Adds the given string to the log buffer. If you are
 * passing data faster then the writing loop can write to
 * file, this function will block the caller to dump a
 * buffer to file.
 * @param log - The DATA_LOGGER structure to use
 * @param message - The message to have logged
 * @return -1 if log is NULL, 0 if message was added to
 * buffer
 */
int data_logger_log(DATA_LOGGER *log, const char* message);

/**
 * This will shut down the logger and write all
 * data from the buffers, close the file, and
 * free all allocated memory.
 * @param log - The logger to close
 * @return The return from fclose. 0 for success
 */
int data_logger_shutdown(DATA_LOGGER *log);

#endif
