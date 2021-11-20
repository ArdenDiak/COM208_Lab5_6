#include "include/csapp.h"
#include "include/connex.h"

#define MAX_UNIQUE_WORDS 1000

typedef struct {
  int length;
  int numUniqueW;
  char *unique_w;
  int *unique_w_occ;
} fileStats;

void echo(int connfd);

void fileStatsInit(fileStats *stats);

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


    Rio_readinitb(&rio, connfd);
    while((n = Rio_readlineb(&rio, buf, MAXLINE)) != 0) {
        //printf("server received %d bytes\n", (int)n);
        //rio_writen(connfd, buf, n);
        if(!strcmp((const char*)&endTX, buf)) {
            printf("transmission ends here\n");
            break;
        }
        getStats(buf, Stats)
    }
}

int binSearch(char *list, int N, char *target){
    if(N == 0) return -1;

    int hi= N;
    int lo= 0;
    int mid= (hi-lo)/2;

    while(lo < hi){
        mid= lo + (hi-lo)/2;
        if(strcmp(list[mid], target) == 0)
            return mid;
        else if(strcmp(list[mid], target) < 0)
            lo= mid + 1;
        else
            hi= mid;  //not `hi = mid-1` (check loop invariants)
        }
        /* no need for these conditions either, check proof
        if(strcmp(list[hi], target) != 0 && hi == (lo+1)){
            return -1;
        }
        */
}

void getStats(char *line, fileStats *stats){
    int w_idx;
    stats->length = (int)(strlen(line) - 1);

    char* token = strtok(line, " ");
    while (token != NULL) {
        //make token lowercase
        int i;
        while(token[i]){
            token[i]= tolower(token[i]);
            i++;
        }

        if((w_idx = binSearch(stats->unique_w, stats->numUniqueW, token)) < 0){
            //add new word, sort list, incr. occ. count
            stats->unique_w[w_idx]= token;
            stats->numUniqueW++;
            mergeSort(stats->unique_w, stats-);

            stats->unique_w_occ[w_idx]++;
        }else{ //ele found in list
            stats->unique_w_occ[w_idx]++;
        }

        token = strtok(NULL, " ");
    }
}

void fileStatsInit(fileStats *stats){
    stats->unique_w = malloc(sizeof(char) * MAX_UNIQUE_WORDS);
    stats->unique_w_occ = malloc(sizeof(int) * MAX_UNIQUE_WORDS);
    stats->length = 0;
    stats->numUniqueW = 0;
}

void fileStatsClean(fileStats *stats){
    free(stats->unique_w);
    free(stats->unique_w_occ);
}
