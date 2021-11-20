#include "include/csapp.h"
#include "include/connex.h"

int main(int argc, char **argv){
    int clientfd;
    char buf[MAXLINE];
    rio_t rio;

    int filename_len= 200;
    char *filename= malloc(sizeof(char)*filename_len);

    if(argc != 3){
        fprintf(stderr, "usage: %s <host> <port>\n", argv[0]);
        exit(0);
    }

    clientfd= open_clientfd(argv[1], argv[2]); //argv[0] is the exec!!!!!
    rio_readinitb(&rio, clientfd);

    printf("input filename: ");
    fgets(filename, 200, stdin);
    filename[strcspn(filename, "\n")] = 0; //remove last /n

    int fd, bytesRead;

    if ((fd = open(filename, O_RDONLY)) < 0) {
        perror("open");
        exit(1);
    }

    while((bytesRead= read(fd, buf, MAXLINE)) != 0) {
        rio_writen(clientfd, buf, bytesRead);
        rio_readlineb(&rio, buf, MAXLINE);
        fputs(buf, stdout);
    }

    close(clientfd);
    exit(0);
}

