STRUCT=Data_Structures/

CC=clang
CFLAGS=-g -Wall -std=gnu11
OFLAGS=-c -fPIC -I$(STRUCT)

IO_SRC=$(wildcard BBBio/*.c) 
IO_OBJ=$(IO_SRC:.c=.o)

LIBS=-llogger -lconfig -lBBBio -ldata_logger
RPATH=-Wl,-rpath,Output,-rpath,Output/Graphics,-rpath,Output/BBBio
XFLAGS=`pkg-config --libs x11`
LINKCOM=-IOutput -IOutput/BBBio -LOutput -LOutput/BBBio


all: config logger BBBio data_logger

#Data Structure(s) Build
link: $(STRUCT)link.c $(STRUCT)link.h
	$(CC) $(CFLAGS) $(OFLAGS) $(STRUCT)link.c -o $(STRUCT)link.o
	cp $(STRUCT)link.h Output/link.h

queue: $(STRUCT)queue.c $(STRUCT)queue.h
	$(CC) $(CFLAGS) $(OFLAGS) $(STRUCT)queue.c -o $(STRUCT)queue.o
	cp $(STRUCT)queue.h Output/queue.h

sort: $(STRUCT)sorted_list.c $(STRUCT)sorted_list.h
	$(CC) $(CFLAGS) $(OFLAGS) $(STRUCT)sorted_list.c -o $(STRUCT)sorted_list.o
	cp $(STRUCT)sorted_list.h Output/sorted_list.h

#BBB gpio Library
BBBio: $(IO_OBJ)
	mkdir -p Output/BBBio
	$(CC) -shared -o Output/BBBio/libBBBio.so $(IO_OBJ)
	cp BBBio/*.h Output/BBBio

$(IO_OBJ): $(IO_SRC)
	$(CC) $(CFLAGS) $(OFLAGS) $(subst .o,.c,$@) -o $@

#Config Builder
config: link Config/config.c Config/config.h
	$(CC) $(CFLAGS) $(OFLAGS) Config/config.c -o Config/config.o
	$(CC)  -shared -o Output/libconfig.so Config/config.o $(STRUCT)link.o 
	cp Config/config.h Output/config.h

#Logger Builder 
logger: queue Logger/logger.c Logger/logger.h  
	$(CC) $(CFLAGS) $(OFLAGS) Logger/logger.c -o Logger/logger.o
	$(CC) -shared -o Output/liblogger.so Logger/logger.o $(STRUCT)queue.o -lpthread
	cp Logger/logger.h Output/logger.h

#Data Logger Builder 
data_logger: queue Logger/data_logger.c Logger/data_logger.h  
	$(CC) $(CFLAGS) $(OFLAGS) Logger/data_logger.c -o Logger/data_logger.o
	$(CC) -shared -o Output/libdata_logger.so Logger/data_logger.o $(STRUCT)queue.o -lpthread
	cp Logger/data_logger.h Output/data_logger.h

