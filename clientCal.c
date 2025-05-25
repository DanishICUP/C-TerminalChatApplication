#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>

void error(const char *msg) {
    perror(msg);
    exit(0);
}

int main(int argc, char *argv[]) {
    int sockfd, portno, n;
    struct sockaddr_in serv_addr;
    struct hostent *server;
    
    char buffer[255];
    if (argc < 3) {
        fprintf(stderr, "usage %s hostname port\n", argv[0]);
        exit(0);
    }
    portno = atoi(argv[2]);
    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd < 0) error("Error opening socket");

    server = gethostbyname(argv[1]);
    if (server == NULL) {
        fprintf(stderr, "Error, no such host\n");
        exit(0);
    }

    bzero((char *)&serv_addr, sizeof(serv_addr));
    serv_addr.sin_family = AF_INET;
    bcopy((char *)server->h_addr_list[0], (char *)&serv_addr.sin_addr, server->h_length);
    serv_addr.sin_port = htons(portno);
    if (connect(sockfd, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0)
        error("Connection failed");

   
    int num1, num2, answer, choice;

    s: bzero(buffer , 255);
    n = read(sockfd , buffer , 255);
    if(n < 0) error("ERROR, reading from socket");
    printf("server: %s\n", buffer);
    scanf("%d" , &num1);
    write(sockfd , &num1 , sizeof(int));

    bzero(buffer , 255);
    n = read(sockfd , buffer , 255);
    if(n < 0) error("ERROR, reading from socket");
    printf("server: %s\n", buffer);
    scanf("%d" , &num2);
    write(sockfd , &num2 , sizeof(int));

    bzero(buffer , 255);
    n = read(sockfd , buffer , 255);
    if(n < 0) error("ERROR, reading from socket");
    printf("server: %s\n", buffer);
    scanf("%d" , &choice);
    write(sockfd , &choice , sizeof(int));

    if(choice == 5) goto Q;

    read(sockfd , &answer , sizeof(int));
    printf("SERVER: ans => is  %d\n",answer);

    if(choice != 5) goto s;

    Q:
    printf("You have selected to exit: Exit Successfuly ");
    close(sockfd);
    return 0;
}
