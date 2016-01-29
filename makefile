STRUCT=Data_Structures/
GRAPHC=Graphics/code/
GRAPHO=Graphics/objects/

CC=clang
CFLAGS=-g -Wall -std=gnu11
OFLAGS=-c -fPIC -I$(STRUCT)

GRAPHICS_SRC=$(wildcard Graphics/code/graphics_*.c)
GRAPHICS_INC=-IGraphics/headers/
GRAPHICS_OBJ=$(addprefix Graphics/objects/,$(notdir $(GRAPHICS_SRC:.c=.o)))

IO_SRC=$(wildcard BBBio/*.c) 
IO_OBJ=$(IO_SRC:.c=.o)

LIBS=-lgraphics -llogger -lconfig -lBBBio -ldata_logger
RPATH=-Wl,-rpath,Output,-rpath,Output/Graphics,-rpath,Output/BBBio
XFLAGS=`pkg-config --libs x11`
LINKCOM=-IOutput -IOutput/Graphics -IOutput/BBBio -LOutput -LOutput/Graphics -LOutput/BBBio

all: config logger graphics BBBio data_logger test

#Build Test code
test: test.c
	$(CC) $(CFLAGS) $(LINKCOM) $(RPATH) test.c -o main $(LIBS)

#Data Structure(s) Build
link: $(STRUCT)link.c $(STRUCT)link.h
	$(CC) $(CFLAGS) $(OFLAGS) $(STRUCT)link.c -o $(STRUCT)link.o
	cp $(STRUCT)link.h Output/link.h

queue: $(STRUCT)queue.c $(STRUCT)queue.h
	$(CC) $(CFLAGS) $(OFLAGS) $(STRUCT)queue.c -o $(STRUCT)queue.o
	cp $(STRUCT)queue.h Output/queue.h

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
#Graphics Build
graphics: link $(GRAPHICS_OBJ) $(GRAPHC)graphics.c
	mkdir -p Output/Graphics
	$(CC) $(CFLAGS) $(OFLAGS) $(GRAPHICS_INC) $(GRAPHC)graphics.c -o $(GRAPHO)graphics.o
	$(CC) -shared -o Output/Graphics/libgraphics.so $(GRAPHICS_OBJ) $(STRUCT)link.o $(GRAPHO)graphics.o -lpthread -lXpm $(XFALGS)
	cp Graphics/headers/*.h Output/Graphics/

$(GRAPHICS_OBJ): $(GRAPHICS_SRC)
	$(CC) $(CFLAGS) $(OFLAGS) $(GRAPHICS_INC) $(subst .o,.c,$(subst objects,code,$@)) -o $(subst code,objects,$@)
