#include <sys/socket.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

int main()
{
    int sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd < 0)
    {
        printf("Failed to open socket");
        exit(EXIT_FAILURE);
    };

    printf("Socket opened with descriptor: %d\n", sockfd);

    close(sockfd);
    return 0;
}