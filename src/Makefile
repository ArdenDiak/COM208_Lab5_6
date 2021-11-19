CC= gcc
CFLAGS= -I$(IDIR)

IDIR= ../include
DEPS= $(wildcard $(IDIR)/*.h)

SRCDIR= ../src
SRC_FILES = $(wildcard $(SRC_DIR)/*.c)

OBJDIR= ../obj
OBJ_FILES = $(wildcard $(OBJDIR)/*.o)

OBJ_FILES: $(SRC_DIR)/%.c
	$(CC) -c -o $@ $< $(CFLAGS)


#Echo Client files
ECHO_OBJ_FILES= ../obj/echo.o

ECHO_OBJ_FILES: ../echo.c
	$(CC) -c -o $@ $< $(CFLAGS)

#echoclient depends on object files (perhaps remove cflags)
echoclient: $(ECHO_OBJ_FILES) $(OBJ_FILES)
	$(CC) -o $@ $^ $(CFLAGS)


#delete unecessary object files
clean:
	$(NO_ECHO) rm -f $(OBJ_DIR)/*.o
