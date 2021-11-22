#include "../include/csapp.h"

#define BUFSIZE 64

int main(int argc, char **argv)
{
    int fd, bytesRead;
    char buf[BUFSIZE];
    int filename_len= 200;
    char *filename= malloc(sizeof(char)*filename_len);

    printf("input filename: ");
    fgets(filename, 200, stdin);
    filename[strcspn(filename, "\n")] = 0; //remove last /n

    if ((fd = open(filename, O_RDONLY)) < 0) {
        perror("open");
        exit(1);
    }

    while((bytesRead= read(fd, buf, BUFSIZE)) != 0)
        write(1, buf, bytesRead);

    return 0;
}
