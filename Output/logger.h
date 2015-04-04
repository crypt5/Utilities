#ifndef LOGGER_H_
#define LOGGER_H_

typedef struct logger_t LOGGER;

LOGGER *logger_init(char* filename);
void logger_log(LOGGER *log,char* message);
void logger_shutdown(LOGGER *log);

#endif
