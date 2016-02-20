#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>
#include <string.h>
#include <time.h>
#include "logger.h"
#include "queue.h"

struct logger_t {
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
    LOGGER* log = NULL;
    log = (LOGGER*) data;
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
        /* Micro * milli */
        usleep(update);
    }
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
 * Creates a new log file with the set filename
 * @param filename - The filename and path to use
 * @param buffer_size - The desired size of the buffer. a value of 0
 * results in the default 4k buffer
 * @return LOGGER* on success, NULL if malloc,
 * mutex, or thread creation failed
 */
LOGGER *logger_init(const char* filename, unsigned int buffer_size,
        unsigned int update_int)
{
    unsigned int size;
    if (buffer_size == 0)
        size = 4096;
    else
        size = buffer_size;

    LOGGER* log = NULL;
    log = malloc(sizeof(LOGGER));
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
 * Adds the message to the logger queue
 * @param log - The logger to use
 * @param message - The message to add
 *    the logger will add the time and data to the
 *    beginning of the line.
 * @return -1 if log is NULL, -2 if message + time and date
 *  is longer then 1024 and malloc fails, and 0 on success
 */
int logger_log(LOGGER *log, const char* message)
{
    char string[1024];
    char* buf = NULL;
    int i;
    char time_out[100];
    time_t rawtime;

    if (log == NULL) {
        return -1;
    }

    time(&rawtime);
    sprintf(time_out, "%s", ctime(&rawtime));
    time_out[strlen(time_out) - 1] = '\0';

    if ((strlen(time_out) + strlen(message) + 4) > 1024) {
        printf("Allocating String\n");
        buf = malloc(sizeof(char) * (strlen(time_out) + strlen(message) + 4));
        if (buf == NULL)
            return -2;

        buf[0] = '[';
        for (i = 0; i < strlen(time_out); i++)
            buf[i + 1] = time_out[i];
        buf[i + 1] = ']';
        buf[i + 2] = ' ';
        buf[i + 3] = '\0'; /* For str cat to work */

        strcat(buf, message);

        pthread_mutex_lock(&log->lock);
        if (log->cur_buffer == 1) {
            if (log->buf_1_pos + strlen(buf) < log->buf_size) {
                log->buf_1_pos = log->buf_1_pos + strlen(buf);
                strcat(log->buffer1, buf);
            } else {
                if (log->buf_2_pos != 0) { //Both buffers are full. Dump data NOW
                    fprintf(log->log, "%s", log->buffer2);
                    log->buffer2[0] = 0;
                    log->buf_2_pos = 0;
                }
                int dif = log->buf_size - log->buf_1_pos - 1;
                strncat(log->buffer1, buf, dif);
                log->buf_1_pos = log->buf_size;
                log->buf_2_pos = strlen(buf) - dif;
                strcat(log->buffer2, &buf[dif]);
                log->cur_buffer = 2;
            }
        } else {
            if (log->buf_2_pos + strlen(buf) < log->buf_size) {
                log->buf_2_pos = log->buf_2_pos + strlen(buf);
                strcat(log->buffer2, buf);
            } else {
                if (log->buf_1_pos != 0) { //Both buffers are full. Dump data NOW
                    fprintf(log->log, "%s", log->buffer1);
                    log->buffer1[0] = 0;
                    log->buf_1_pos = 0;
                }
                int dif = log->buf_size - log->buf_2_pos - 1;
                strncat(log->buffer2, buf, dif);
                log->buf_2_pos = log->buf_size;
                log->buf_1_pos = strlen(buf) - dif;
                strcat(log->buffer1, &buf[dif]);
                log->cur_buffer = 1;
            }
        }
        pthread_mutex_unlock(&log->lock);
        free(buf);

    } else {
        string[0] = '[';
        for (i = 0; i < strlen(time_out); i++)
            string[i + 1] = time_out[i];
        string[i + 1] = ']';
        string[i + 2] = ' ';
        string[i + 3] = '\0'; /* For str cat to work */
        strcat(string, message);

        pthread_mutex_lock(&log->lock);
        if (log->cur_buffer == 1) {
            if ((log->buf_1_pos + strlen(string)) < log->buf_size) {
                log->buf_1_pos = log->buf_1_pos + strlen(string);
                strcat(log->buffer1, string);
            } else {
                if (log->buf_2_pos != 0) { //Both buffers are full. Dump data NOW
                    fprintf(log->log, "%s", log->buffer2);
                    log->buffer2[0] = 0;
                    log->buf_2_pos = 0;
                }
                int dif = log->buf_size - log->buf_1_pos - 1;
                strncat(log->buffer1, string, dif);
                log->buf_1_pos = log->buf_size;
                log->buf_2_pos = strlen(string) - dif;
                strcat(log->buffer2, &string[dif]);
                log->cur_buffer = 2;
            }
        } else {
            if ((log->buf_2_pos + strlen(string)) < log->buf_size) {
                log->buf_2_pos = log->buf_2_pos + strlen(string);
                strcat(log->buffer2, string);
            } else {
                if (log->buf_1_pos != 0) { //Both buffers are full. Dump data NOW
                    fprintf(log->log, "%s", log->buffer1);
                    log->buffer1[0] = 0;
                    log->buf_1_pos = 0;
                }
                int dif = log->buf_size - log->buf_2_pos - 1;
                strncat(log->buffer2, string, dif);
                log->buf_2_pos = log->buf_size;
                log->buf_1_pos = strlen(string) - dif;
                strcat(log->buffer1, &string[dif]);
                log->cur_buffer = 1;
            }
        }
        pthread_mutex_unlock(&log->lock);
    }

    return 0;
}

/**
 * Shuts down the logger and adds the remaining
 * lines to the log file
 * @param log - Logger to shutdown
 * @return 0 on success
 */
int logger_shutdown(LOGGER *log)
{
    pthread_mutex_lock(&log->lock);
    log->run = 0;
    pthread_mutex_unlock(&log->lock);
    pthread_join(log->thread, NULL);
    fclose(log->log);
    pthread_mutex_destroy(&log->lock);
    free(log->buffer1);
    free(log->buffer2);
    free(log);

    return 0;

}
