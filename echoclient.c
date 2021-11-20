#include "include/csapp.h"
#include "include/connex.h"

int main(int argc, char **argv){
    int clientfd;
    char buf[MAXLINE];
    rio_t rio;

    if(argc != 3){
        fprintf(stderr, "usage: %s <host> <port>\n", argv[0]);
        exit(0);
    }

    clientfd= open_clientfd(argv[1], argv[2]); //argv[0] is the exec!!!!!
    rio_readinitb(&rio, clientfd);

    while(fgets(buf, MAXLINE, stdin) != NULL)
    {
        rio_writen(clientfd, buf, strlen(buf));
        rio_readlineb(&rio, buf, MAXLINE);
        fputs(buf, stdout);
    }
    close(clientfd);
    exit(0);
}

