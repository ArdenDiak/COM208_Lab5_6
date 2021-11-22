#include "include/csapp.h"
#include "include/connex.h"

int main(int argc, char **argv){

    int clientfd, in_fd, bytesRead;
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
        if ((in_fd = open(filename, O_RDONLY)) < 0) {
            perror("fopen");
            exit(1);
        }

        rio_readinitb(&rio_cli, clientfd);
        rio_readinitb(&rio_file, in_fd);

        //transmit all lines of the file to srv
        int n;
        while((n = Rio_readlineb(&rio_file, buf, MAXLINE)) != 0)
        {
            rio_writen(clientfd, buf, strlen(buf));
            printf(">>%d\n", n);
        }
        printf("sent file");

        //notify SRV that file transmit is done
        char endFile[10]= "END FILE";
        rio_writen(clientfd, endFile, strlen(endFile));
        printf("sent END FILE");

        //await response
        while((bytesRead= rio_readlineb(&rio_cli, buf, MAXLINE)) != 0) {
            printf("response >> %s\n", buf);
        }

        printf("filename: ");
    }

    free(filename);
    close(clientfd);
    exit(0);
}
