#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>
#include <string.h>
#include <time.h>
#include "data_logger.h"
#include "queue.h"

struct data_logger_t {
        pthread_mutex_t lock;
        pthread_t thread;
        char* buffer1;
        char* buffer2;
        int buf_1_pos;
        int buf_2_pos;
        unsigned int buf_size;
        unsigned int update_int;
        int cur_buffer;
        FILE* log;
        int run;
};

// The loop the logger thread runs
void* loop(void* data)
{
    DATA_LOGGER* log = NULL;
    log = (DATA_LOGGER*) data;
    pthread_mutex_lock(&log->lock);
    char runner = (char) log->run;
    unsigned int update = log->update_int;
    pthread_mutex_unlock(&log->lock);

    while (runner) {
        pthread_mutex_lock(&log->lock);
        if (log->cur_buffer == 2) {
            if (log->buf_1_pos == log->buf_size) {
                fprintf(log->log, "%s", log->buffer1);
                log->buffer1[0] = '\0';
                log->buf_1_pos = 0;
            }
        } else {
            if (log->buf_2_pos == log->buf_size) {
                fprintf(log->log, "%s", log->buffer2);
                log->buffer2[0] = '\0';
                log->buf_2_pos = 0;
            }
        }
        runner = log->run;
        pthread_mutex_unlock(&log->lock);
        usleep(update);
    }
    //Shutting down So dump buffers
    pthread_mutex_lock(&log->lock);
    if (log->cur_buffer == 2) {
        if (log->buf_2_pos != 0) {
            fprintf(log->log, "%s", log->buffer2);
        }
    } else {
        if (log->buf_1_pos != 0) {
            fprintf(log->log, "%s", log->buffer1);
        }
    }
    pthread_mutex_unlock(&log->lock);
    return NULL;
}

/**
 * Sets up a new data logger
 * @param filename - The filename to use for logging
 * @return DATA_LOGGER* on success, NULL on failure
 */
DATA_LOGGER *data_logger_init(const char* filename, unsigned int buffer_size,
        unsigned int update_int)
{
    unsigned int size;
    if (buffer_size == 0)
        size = 4096;
    else
        size = buffer_size;

    DATA_LOGGER* log = NULL;
    log = malloc(sizeof(DATA_LOGGER));
    if (log == NULL) {
        return NULL;
    }

    log->buffer1 = malloc(sizeof(char) * size);
    if (log->buffer1 == NULL) {
        return NULL;
    }

    log->buffer2 = malloc(sizeof(char) * size);
    if (log->buffer2 == NULL) {
        return NULL;
    }

    if (pthread_mutex_init(&log->lock, NULL) == 1) {
        return NULL;
    }

    log->buffer1[0] = '\0';
    log->buffer2[0] = '\0';
    log->log = fopen(filename, "w");
    log->cur_buffer = 1;
    log->buf_1_pos = 0;
    log->buf_2_pos = 0;
    log->buf_size = size;
    log->update_int = update_int;
    log->run = 1;

    if (pthread_create(&log->thread, NULL, &loop, log) == 1) {
        return NULL;
    }

    return log;
}

/**
 * Adds the message to the logging queue
 * @param log - The data logger to use
 * @param message - The data to add to the log
 * @return 0 on success, -1 for message malloc fail
 * -2 for enqueue fail
 */
int data_logger_log(DATA_LOGGER *log, const char* message)
{
    if (log == NULL) {
        return -1;
    }

    pthread_mutex_lock(&log->lock);
    if (log->cur_buffer == 1) {
        if ((log->buf_1_pos + strlen(message)) < log->buf_size) {
            log->buf_1_pos = log->buf_1_pos + strlen(message);
            strcat(log->buffer1, message);
        } else {
            if (log->buf_2_pos != 0) { //Both buffers are full. Dump data NOW
                fprintf(log->log, "%s", log->buffer2);
                log->buffer2[0] = 0;
                log->buf_2_pos = 0;
            }
            int dif = log->buf_size - log->buf_1_pos - 1;
            strncat(log->buffer1, message, dif);
            log->buf_1_pos = log->buf_size;
            log->buf_2_pos = strlen(message) - dif;
            strcat(log->buffer2, &message[dif]);
            log->cur_buffer = 2;
        }
    } else {
        if ((log->buf_2_pos + strlen(message)) < log->buf_size) {
            log->buf_2_pos = log->buf_2_pos + strlen(message);
            strcat(log->buffer2, message);
        } else {
            if (log->buf_1_pos != 0) { //Both buffers are full. Dump data NOW
                fprintf(log->log, "%s", log->buffer1);
                log->buffer1[0] = 0;
                log->buf_1_pos = 0;
            }
            int dif = log->buf_size - log->buf_2_pos - 1;
            strncat(log->buffer2, message, dif);
            log->buf_2_pos = log->buf_size;
            log->buf_1_pos = strlen(message) - dif;
            strcat(log->buffer1, &message[dif]);
            log->cur_buffer = 1;
        }
    }
    pthread_mutex_unlock(&log->lock);

    return 0;
}

/**
 * Shuts down the logger and writes the remaining data to file
 * @param log - The logger to use
 * @return 0 on success, -1 if data freeing failed.
 */
int data_logger_shutdown(DATA_LOGGER *log)
{
    int re = 0;
    pthread_mutex_lock(&log->lock);
    log->run = 0;
    pthread_mutex_unlock(&log->lock);
    pthread_join(log->thread, NULL);
    re = fclose(log->log);
    pthread_mutex_destroy(&log->lock);
    free(log->buffer1);
    free(log->buffer2);
    free(log);

    return re;
}
