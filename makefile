INSTALL=/usr/local

STRUCT=Data_Structures/

CC=clang
CFLAGS=-g -Wall -std=gnu11
OFLAGS=-c -fPIC -I$(STRUCT)

IO_SRC=$(wildcard BBBio/*.c) 
IO_OBJ=$(IO_SRC:.c=.o)


all: data config logger BBBio data_logger

# Compile command for test File
test: test.c
	$(CC) $(CFLAGS) test.c -o main -ldata_logger

#Data Structure(s) Build
data: link queue sort
	$(CC) -shared -o libdata.so link.o queue.o sorted_list.o

link: $(STRUCT)link.c $(STRUCT)link.h
	$(CC) $(CFLAGS) $(OFLAGS) $(STRUCT)link.c -o link.o

queue: $(STRUCT)queue.c $(STRUCT)queue.h
	$(CC) $(CFLAGS) $(OFLAGS) $(STRUCT)queue.c -o queue.o

sort: $(STRUCT)sorted_list.c $(STRUCT)sorted_list.h
	$(CC) $(CFLAGS) $(OFLAGS) $(STRUCT)sorted_list.c -o sorted_list.o

#BBB gpio Library
BBBio: $(IO_OBJ)
	$(CC) -shared -o libBBBio.so $(IO_OBJ)

$(IO_OBJ): $(IO_SRC)
	$(CC) $(CFLAGS) $(OFLAGS) $(subst .o,.c,$@) -o $@

#Config Builder
config: data Config/config.c Config/config.h
	$(CC) $(CFLAGS) $(OFLAGS) Config/config.c -o config.o
	$(CC)  -shared -o libconfig.so config.o link.o 

#Logger Builder 
logger: Logger/logger.c Logger/logger.h  
	$(CC) $(CFLAGS) $(OFLAGS) Logger/logger.c -o logger.o
	$(CC) -shared -o liblogger.so logger.o -lpthread

#Data Logger Builder 
data_logger: Logger/data_logger.c Logger/data_logger.h  
	$(CC) $(CFLAGS) $(OFLAGS) Logger/data_logger.c -o data_logger.o
	$(CC) -shared -o libdata_logger.so data_logger.o -lpthread


# Removes all compiled files
clean:
	rm *.so
	rm *.o
	rm BBBio/*.o

# Will install everything
install: install-data install-BBBio install-config install-logger install-data_logger

# Puts the library files in the right place
install-data: data
	cp libdata.so $(INSTALL)/lib
	mkdir -p $(INSTALL)/include/kenutil
	cp Data_Structures/*.h $(INSTALL)/include/kenutil
	
install-BBBio: BBBio
	cp libBBBio.so $(INSTALL)/lib/
	mkdir -p $(INSTALL)/include/BBBio
	cp BBBio/*.h $(INSTALL)/include/BBBio

install-config: config
	cp libconfig.so $(INSTALL)/lib
	mkdir -p $(INSTALL)/include/kenutil
	cp Config/*.h $(INSTALL)/include/kenutil

install-logger: logger
	cp liblogger.so $(INSTALL)/lib
	mkdir -p $(INSTALL)/include/kenutil
	cp Logger/*.h $(INSTALL)/include/kenutil

install-data_logger: data_logger
	cp libdata_logger.so $(INSTALL)/lib
	mkdir -p $(INSTALL)/include/kenutil
	cp Logger/*.h $(INSTALL)/include/kenutil

