#include "include/csapp.h"
#include "include/connex.h"

int main(int argc, char **argv){

    int clientfd, input_filefd, bytesRead;
    char buf[MAXLINE];
    rio_t rio_cli, rio_file;

    int filename_len= 200;
    char *filename= malloc(sizeof(char)*filename_len);

    if(argc != 3){
        fprintf(stderr, "usage: %s <host> <port>\n", argv[0]);
        exit(0);
    }

    clientfd= open_clientfd(argv[1], argv[2]); //argv[0] is the exec!!!!!

    printf("filename: ");
    while(1) {
        /* Get filename from user to send to server */
        fgets(filename, 200, stdin);
        if(strlen(filename) == 1 && filename[0] == '\n') {
            char endTX[10]="END TX";
            rio_writen(clientfd, endTX, strlen(endTX));
            printf("No filename inputted => end transmission\n");
            break;
        }

        filename[strcspn(filename, "\n")] = 0; //remove last /n

        //open the requesed for reading
        if ((input_filefd = open(filename, O_RDONLY)) < 0) {
            perror("open");
            exit(1);
        }

        rio_readinitb(&rio_cli, clientfd);
        rio_readinitb(&rio_file, input_filefd);

        //transmit all lines of the file to srv
        while((bytesRead= rio_readlineb(&rio_file, buf, MAXLINE)) != 0) {
            rio_writen(clientfd, buf, strlen(buf));
            //rio_readlineb(&rio_cli, buf, MAXLINE);
            //fputs(buf, stdout);
        }

        //await response
        while((bytesRead= rio_readlineb(&rio_file, buf, MAXLINE)) != 0) {
            fputs(buf, stdout);
        }

        printf("filename: ");
    }

    free(filename);
    close(clientfd);
    exit(0);
}
