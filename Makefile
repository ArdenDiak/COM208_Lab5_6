CC= gcc
CFLAGS= -I$(IDIR)

IDIR= include
DEPS= $(wildcard $(IDIR)/*.h)

SRCDIR= src
SRC_FILES = $(wildcard $(SRC_DIR)/*.c)

OBJDIR= obj
OBJ_FILES = $(patsubst %.c, $(OBJDIR)/%.o, $(SRC_FILES))

OBJ_FILES: $(SRC_FILES)
	$(CC) -c -o $@ $< $(CFLAGS)

#Echo Client files
ECHO_OBJ_FILES= obj/echoclient.o

ECHO_OBJ_FILES: echoclient.c
	$(CC) -c $< -o $@ $(CFLAGS)

#echoclient depends on object files (perhaps remove cflags)
echoclient: $(ECHO_OBJ_FILES) $(OBJ_FILES)
	$(CC) -o $@ $^ $(CFLAGS)


#Echo Server Files
ECHOSRV_OBJ_FILES= obj/echoserver.o

ECHOSRV_OBJ_FILES: echoserver.c
	$(CC) -c -o $@ $< $(CFLAGS)

#echoserver depends on object files (perhaps remove cflags)
echoserver: $(ECHOSRV_OBJ_FILES) $(OBJ_FILES)
	$(CC) -o $@ $^ $(CFLAGS)


#delete unecessary object files
clean:
	$(NO_ECHO) rm -f $(OBJ_DIR)/*.o
