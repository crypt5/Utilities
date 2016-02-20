#ifndef LOGGER_H_
#define LOGGER_H_

typedef struct logger_t LOGGER;

/**
 * Creates a new log file with the set filename
 * @param filename - The path a filename to use
 * @param buffer_size - The desired size of the buffer. a value of 0
 * results in the default 4k buffer
 * @param update_int - The amount of time(micro Seconds) the writing
 * thread should wait between runs. This value should be as long as
 * possible, but not so long that both buffers fill up. this will
 * cause the logger_log function to block the caller while it dumps
 *  a buffer
 * @return LOGGER* on success, NULL if malloc,
 * mutex, or thread creation failed
 */
LOGGER *logger_init(const char* filename, unsigned int buffer_size,
        unsigned int update_int);

/**
 * Adds the message to the logger buffer. The logger will
 * append the message to the date and time before it writes
 * the information to file.
 * @param log - The logger to use
 * @param message - The message to add. If the length of
 * the message is greater then 1000 characters the logger
 * will be forced to allocate a different buffer to use.
 * This will slow down the calling thread.
 * @return -1 if log is NULL, -2 if message + time and date
 *  is longer then 1024 and malloc fails, and 0 on success
 */
int logger_log(LOGGER *log, const char* message);

/**
 * Shuts down the logger and dumps the buffers to file
 * @param log - Logger to shutdown
 * @return 0 on success, error from fclose if the
 * file did not close correctly
 */
int logger_shutdown(LOGGER *log);

#endif
