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
    QUEUE* lines;
    FILE* log;
    char run;
};

/* Needed by queue library */
void my_free(void* data) {
    free(data);
}

/**
 * Loop that the data logger thread runs
 * @param data - A pointer to the data_logger_t strut
 */
void* loop(void* data) {
    DATA_LOGGER* log = NULL;
    char* temp = NULL;
    char runner = 1;

    log = (DATA_LOGGER*) data;

    while (runner) {
        pthread_mutex_lock(&log->lock);
        runner = log->run;
        if (is_queue_empty(log->lines) != 1) {
            temp = (char*) dequeue(log->lines);
            fprintf(log->log, "%s\n", temp);
            free(temp);
        }

        if (runner == 0) {
            while (is_queue_empty(log->lines) != 1) {
                temp = (char*) dequeue(log->lines);
                fprintf(log->log, "%s\n", temp);
                free(temp);
            }
        }

        pthread_mutex_unlock(&log->lock);
        /* one milli */
        usleep(1000);
    }

    return NULL;
}

/**
 * Sets up a new data logger
 * @param filename - The filename to use for logging
 * @return DATA_LOGGER* on success, NULL on failure
 */
DATA_LOGGER *data_logger_init(char* filename) {
    DATA_LOGGER* log = NULL;

    log = malloc(sizeof(DATA_LOGGER));
    if (log == NULL) {
        return NULL;
    }

    log->log = fopen(filename, "w");
    log->lines = init_queue(my_free);
    log->run = 1;

    if (pthread_mutex_init(&log->lock, NULL) == 1) {
        return NULL;
    }

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
int data_logger_log(DATA_LOGGER *log, const char* message) {
    char *sub;
    sub = malloc(strlen(message) + 1);
    if(sub==NULL)
        return -1;
    strcpy(sub, message);
    pthread_mutex_lock(&log->lock);
    int re=enqueue(log->lines, sub);
    pthread_mutex_unlock(&log->lock);
    if(re!=0)
        return -2;
    return 0;
}

/**
 * Shuts down the logger and writes the remaining data to file
 * @param log - The logger to use
 * @return 0 on success, -1 if data freeing failed.
 */
int data_logger_shutdown(DATA_LOGGER *log) {
    pthread_mutex_lock(&log->lock);
    log->run = 0;
    pthread_mutex_unlock(&log->lock);
    pthread_join(log->thread, NULL);

    fclose(log->log);
    pthread_mutex_destroy(&log->lock);
    int re=destroy_queue(log->lines);
    free(log);
    if (re!=0)
        return -1;
    return 0;
}
