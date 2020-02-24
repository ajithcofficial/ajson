CC= gcc
CFLAGS= -Werror -Wall -ggdb -c
LINK= gcc
INCL= -I include

ajson: objs/ajson_main.o \
	objs/ajson.o \
	objs/ajson_array.o \
	objs/ajson_string.o \
	objs/ajson_memory.o \
	objs/ajson_error.o 
	$(LINK) objs/ajson_main.o \
	objs/ajson.o \
	objs/ajson_array.o \
	objs/ajson_string.o \
	objs/ajson_memory.o \
	objs/ajson_error.o \
	-lpthread -o ajson

objs/ajson_main.o: src/ajson_main.c
	$(CC) $(INCL) $(CFLAGS) src/ajson_main.c -o objs/ajson_main.o

objs/ajson.o: src/ajson.c
	$(CC) $(INCL) $(CFLAGS) src/ajson.c -o objs/ajson.o

objs/ajson_array.o: src/ajson_array.c
	$(CC) $(INCL) $(CFLAGS) src/ajson_array.c -o objs/ajson_array.o

objs/ajson_string.o: src/ajson_string.c
	$(CC) $(INCL) $(CFLAGS) src/ajson_string.c -o objs/ajson_string.o

objs/ajson_memory.o: src/ajson_memory.c
	$(CC) $(INCL) $(CFLAGS) src/ajson_memory.c -o objs/ajson_memory.o

objs/ajson_error.o: src/ajson_error.c
	$(CC) $(INCL) $(CFLAGS) src/ajson_error.c -o objs/ajson_error.o
