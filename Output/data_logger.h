#ifndef DATA_LOGGER_H_
#define DATA_LOGGER_H_

typedef struct data_logger_t DATA_LOGGER;

DATA_LOGGER *data_logger_init(char* filename);
void data_logger_log(DATA_LOGGER *log,char* message);
void data_logger_shutdown(DATA_LOGGER *log);

#endif
