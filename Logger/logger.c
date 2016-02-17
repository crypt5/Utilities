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
    QUEUE* lines;
    FILE* log;
    char run;
};

/* Needed by queue library */
void my_free(void* data) {
    free(data);
}

// The loop the logger thread runs
void* loop(void* data) {
    LOGGER* log = NULL;
    char* temp = NULL;
    char runner = 1;

    log = (LOGGER*) data;

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
        /* Micro * milli */
        usleep(1000 * 100);
    }

    return NULL;
}

/**
 * Creates a new log file with the set filename
 * @param filename - The filename and path to use
 * @return LOGGER* on success, NULL if malloc,
 * mutex, or thread creation failed
 */
LOGGER *logger_init(char* filename) {
    LOGGER* log = NULL;

    log = malloc(sizeof(LOGGER));
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
 * Adds the message to the logger queue
 * @param log - The logger to use
 * @param message - The message to add
 *    the logger will append the time and data to the
 *    beginning of the line
 * @return -1 if log is NULL, -2 if line malloc fails,
 * -3 if enqueue fails, and 0 on success
 */
int logger_log(LOGGER *log, char* message) {
    char* sub = NULL;
    int i;
    char time_out[1024];
    time_t rawtime;

    time(&rawtime);
    sprintf(time_out, "%s", ctime(&rawtime));
    time_out[strlen(time_out) - 1] = '\0';

    if (log == NULL) {
        return -1;
    }

    sub = malloc(strlen(time_out) + strlen(message) + 4);
    if (sub == NULL) {
        return -2;
    }
    sub[0] = '[';
    for (i = 0; i < strlen(time_out); i++)
        sub[i + 1] = time_out[i];
    sub[strlen(time_out) + 1] = ']';
    sub[strlen(time_out) + 2] = ' ';
    sub[strlen(time_out) + 3] = '\0'; /* For str cat to work */
    strcat(sub, message);

    pthread_mutex_lock(&log->lock);
    int re=enqueue(log->lines, sub);
    pthread_mutex_unlock(&log->lock);
    if(re!=0)
        return -3;
    return 0;
}

/**
 * Shuts down the logger and adds the remaining
 * lines to the log file
 * @param log - Logger to shutdown
 * @return -1 if queue not destroyed correctly,
 * 0 on success
 */
int logger_shutdown(LOGGER *log) {
    pthread_mutex_lock(&log->lock);
    log->run = 0;
    pthread_mutex_unlock(&log->lock);

    pthread_join(log->thread, NULL);

    fclose(log->log);
    pthread_mutex_destroy(&log->lock);
    int re=destroy_queue(log->lines);

    free(log);
    if(re!=0)
        return -1;
    return 0;

}
