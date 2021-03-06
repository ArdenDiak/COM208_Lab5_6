#include "include/csapp.h"
#include "include/connex.h"
#include "include/fileStats.h"

void echo(int connfd);

int main(int argc, char **argv)
{
    int listenfd, connfd;
    socklen_t clientlen;
    struct sockaddr_in clientaddr;
    char client_hostname[MAXLINE], client_port[MAXLINE];

    if (argc != 2) {
        fprintf(stderr, "usage: %s <port>\n", argv[0]);
        exit(0);
    }

    listenfd = open_listenfd(argv[1]);
    while (1) {
        clientlen = sizeof(struct sockaddr_storage);
        connfd = Accept(listenfd, (SA *)&clientaddr, &clientlen);

        /* Determine the domain name and IP address of the client */
        getnameinfo((SA *) &clientaddr, clientlen, client_hostname, MAXLINE, client_port, MAXLINE, 0);
        printf("Connected to (%s, %s)\n", client_hostname, client_port);

        echo(connfd);
        Close(connfd);
    }
    exit(0);
}

void echo(int connfd)
{
    size_t n;
    char buf[MAXLINE];
    rio_t rio;
    char endTX[10]= "END TX";
    char endFile[10]= "END FILE";


    Rio_readinitb(&rio, connfd);
    while((n = Rio_readlineb(&rio, buf, MAXLINE)) != 0) {
        //printf("server received %d bytes\n", (int)n);
        //rio_writen(connfd, buf, n);
        fputs(buf, stdout);
        if(!strcmp((const char*)&endTX, buf)) {
            printf("transmission ends here\n");
            break;
        }

        if(!strcmp((const char*)&endFile, buf)) {
            printf("end msg\n");

            //getStats(buf, Stats)
            char tmp[10]= "Hello\n";
            rio_writen(connfd, tmp, 10);
        }

    }
    printf(">>here");
}

