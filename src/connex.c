#include "../include/csapp.h"

int open_clientfd(char *hostname, char *port)
{
    int clientfd;
    struct addrinfo *p, *listp, hints;
    int rc;

    memset(&hints, 0, sizeof(struct addrinfo));
    hints.ai_socktype = SOCK_STREAM;
    hints.ai_flags = AI_NUMERICSERV | AI_ADDRCONFIG; //numeric port arg

    if((rc = getaddrinfo(hostname, port, &hints, &listp)) != 0){
        printf(">> getaddrinfo exited with error code %s\n", gai_strerror(rc));
        exit(1);
    }

    //walk list of available addrs for given hostname
    for(p=listp; p; p= p->ai_next){
        if((clientfd = socket(p->ai_family, p->ai_socktype, p->ai_protocol)) < 0)
            continue;  //socket fail :(

        //once valid socket found => send connection request
        if(connect(clientfd, p->ai_addr, p->ai_addrlen) != -1)
            break;

        if (close(clientfd) < 0) { /* Connect failed, try another */
            fprintf(stderr, "open_clientfd: close failed: %s\n", strerror(errno));
            return -1;
        }
    }

    /* Clean up */
    freeaddrinfo(listp);
    if(!p)  //all connects failed
        return -1;
    else    //last one succeeded
        return clientfd;
}

int open_listenfd(char *port)
{
    int listenfd, rc, optval=1;
    struct addrinfo *p, *listp, hints;

    memset(&hints, 0, sizeof(struct addrinfo));
    hints.ai_socktype = SOCK_STREAM;
    hints.ai_flags = AI_PASSIVE;                      //accept connex from any valid host
    hints.ai_flags |= AI_NUMERICSERV | AI_ADDRCONFIG; //... on any port number & address

    if((rc = getaddrinfo(NULL, port, &hints, &listp)) != 0) {
        fprintf(stderr, "getaddrinfo failed (port %s): %s\n", port, gai_strerror(rc));
        return -2;
    }

    //walk list of available addrs for given hostname
    for(p=listp; p; p= p->ai_next){
        if((listenfd = socket(p->ai_family, p->ai_socktype, p->ai_protocol)) < 0)
            continue;  //socket fail, go to next

        /* Eliminates "Address already in use" error from bind */
        setsockopt(listenfd, SOL_SOCKET, SO_REUSEADDR, (const void *)&optval , sizeof(int));

        //kernel associates socket addr with sockfd
        if(bind(listenfd, p->ai_addr, p->ai_addrlen) == 0)
            break; //success

        if (close(listenfd) < 0) { /* Connect failed, try another */
            fprintf(stderr, "open_clientfd: close failed: %s\n", strerror(errno));
            return -1;
        }
    }

    /* Clean up */
    freeaddrinfo(listp);
    if(!p) //no address worked
        return -1;

    if(listen(listenfd, LISTENQ) < 0 ){
        close(listenfd);
        return -1;
    }

    return listenfd;
}

