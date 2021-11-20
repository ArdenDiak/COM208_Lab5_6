CC= gcc
CFLAGS= -I $(IDIR)

IDIR= include
DEPS = include/csapp.h include/connex.h
OBJ= src/csapp.o src/connex.o

# $@ and $^, are the left and right sides of the :, respectively
%.o: %.c
	$(CC) -c -o $@ $< $(CFLAGS)

# Echo Client
ECHO_CLI_OBJ= echoclient.o
echoclient: $(ECHO_CLI_OBJ) $(OBJ)
		$(CC) -o $@ $^ $(CFLAGS)

# Echo Server
ECHO_SRV_OBJ= echoserver.o
echoserver: $(ECHO_SRV_OBJ) $(OBJ)
		$(CC) -o $@ $^ $(CFLAGS)

clean:
	$(NO_ECHO) rm -f *.o src/*.o echoclient echoserver
