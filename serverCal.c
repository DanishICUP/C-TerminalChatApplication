#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>

void error(const char *msg)
{
    perror(msg);
    exit(1);
}

int main(int argc, char *argv[])
{
    if (argc < 2)
    {
        fprintf(stderr, "Port not provided. Program terminated!\n");
        exit(1);
    }

    int sockfd, newsockfd, portno, n;
    char buffer[255];
    struct sockaddr_in serv_addr, cli_addr;
    socklen_t clilen;

    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd < 0)
        error("Error opening socket");

    bzero((char *)&serv_addr, sizeof(serv_addr));
    portno = atoi(argv[1]);

    serv_addr.sin_family = AF_INET;
    serv_addr.sin_addr.s_addr = INADDR_ANY;
    serv_addr.sin_port = htons(portno);

    if (bind(sockfd, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0)
        error("Binding failed");

    listen(sockfd, 5);
    clilen = sizeof(cli_addr);

    newsockfd = accept(sockfd, (struct sockaddr *)&cli_addr, &clilen);
    if (newsockfd < 0)
        error("Error on Accept");

    int num1, num2, answer, choice;

s: n = write(newsockfd, "Enter Number 1: ", strlen("Enter Number 1:"));
    if (n < 0)
        error("Error writing to socket");
    read(newsockfd, &num1, sizeof(int));
    printf("Client No 1: %d\n", num1);

    n = write(newsockfd, "Enter Number 2: ", strlen("Enter Number 2:"));
    if (n < 0)
        error("Error writing to socket");
    read(newsockfd, &num2, sizeof(int));
    printf("Client No 2: %d\n", num2);

    n = write(newsockfd, "Enter Your Choice: \n1:Addition\n2:Subtraction\n3:multiplication\n4:Devision\n5:Exit ",
              strlen("Enter Your Choice: \n1:Addition\n2:Subtraction\n3:multiplication\n4:Devision\n5:Exit"));
    if (n < 0)
        error("Error Writing socket");
    read(newsockfd, &choice, sizeof(int));
    printf("Client Choice is: %d\n", choice);

    switch (choice)
    {
    case 1:
        answer = num1 + num2;
        break;
    case 2:
        answer = num1 - num2;
        break;
    case 3:
        answer = num1 * num2;
        break;
    case 4:
        answer = num1 / num2;
        break;
    case 5:
        goto Q;
        break;
    default:
        printf("Wrong Choice please enter in the given");
        break;
    }

    write(newsockfd, &answer, sizeof(int));

    if (choice == 5)
        goto Q;

    if (choice != 5)
        goto s;

Q: close(newsockfd);
    close(sockfd);
    return 0;
}
