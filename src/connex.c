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
        close(clientfd); //connection failed
    }

    freeaddrinfo(listp);
    if(!p)
        return -1;
    else
        return clientfd;
}

int open_listenfd(char *port)
{
    int listenfd, optval=1;
    struct addrinfo *p, *listp, hints;
    int rc;

    memset(&hints, 0, sizeof(struct addrinfo));
    hints.ai_socktype = SOCK_STREAM;
    hints.ai_flags = AI_PASSIVE; //accept connex from any valid host
    hints.ai_flags |= AI_NUMERICSERV | AI_ADDRCONFIG; //numeric port arg

    if((rc = getaddrinfo(NULL, port, &hints, &listp)) != 0){
        printf("getaddrinfo exited with error code %d\n", rc);
        exit(1);
    }

    //walk list of available addrs for given hostname
    for(p=listp; p; p= p->ai_next){
        if((listenfd = socket(p->ai_family, p->ai_socktype, p->ai_protocol)) < 0)
            continue;  //socket fail, go to next

        /* Eliminates "Address already in use" error from bind */
        if (setsockopt(listenfd, SOL_SOCKET, SO_REUSEADDR,
                    (const void *)&optval , sizeof(int)) < 0)
            return -1;

        //kernel associates socket addr with sockfd
        if(bind(listenfd, p->ai_addr, p->ai_addrlen) == 0)
            break; //success
        close(listenfd); //bind failed, try next
    }

    freeaddrinfo(listp);
    if(!p) //no address worked
        return -1;

    if(listen(listenfd, LISTENQ) < 0 ){
        close(listenfd);
        return -1;
    }

    return listenfd;
}

