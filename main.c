#include <sys/socket.h> // socket()
#include <stdio.h>      // input output
#include <stdlib.h>     // exit status code
#include <unistd.h>     // close()
#include <netinet/in.h> // sockaddr_in struct
#include "arraylist.h"
#include "request.h"
#define PORT 8080

int main()
{
    char buffer[1024] = {0};
    struct sockaddr_in server_address;
    struct sockaddr_in client_address;
    socklen_t client_address_len = sizeof(client_address);

    int sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd < 0)
    {
        perror("Failed to open socket");
        exit(EXIT_FAILURE);
    };
    printf("Socket opened with a descriptor: %d\n", sockfd);

    server_address.sin_family = AF_INET;
    server_address.sin_port = htons(PORT);
    server_address.sin_addr.s_addr = INADDR_ANY;

    if (bind(sockfd, (struct sockaddr *)&server_address, sizeof(server_address)) < 0)
    {
        perror("Failed to bind socket");
        close(sockfd);
        exit(EXIT_FAILURE);
    }
    printf("Socket bind successfull\n");

    if (listen(sockfd, 2) < 0)
    {
        perror("cannot listen");
        close(sockfd);
        exit(EXIT_FAILURE);
    }
    printf("Socket listening up to 2 client\n");

    int new_socketfd = accept(sockfd, (struct sockaddr *)&client_address, &client_address_len);
    if (new_socketfd < 0)
    {
        perror("cannot accept");
        close(sockfd);
        exit(EXIT_FAILURE);
    }
    printf("Socket accepted client with a new descriptor: %d\n", new_socketfd);

    ssize_t valread = recv(new_socketfd, buffer, sizeof(buffer) - 1, 0);
    printf("received: %s\n", buffer);
    printf("---------\n");
    printf("---------\n");
    printf("---------\n");
    printf("---------\n");
    printf("---------\n");

    arraylist_t *lines = new_arraylist(2);

    int index = 0;
    int line_char_reader_index = 0;
    while (!(buffer[index - 2] == '\r' && buffer[index - 1] == '\n' && buffer[index] == '\r' && buffer[index + 1] == '\n'))
    {
        int inner = 0;
        char *line = malloc(1024 * sizeof(char));
        while (buffer[index] != '\r')
        {
            line[inner] = buffer[index];
            inner++;
            index++;
        }
        line[inner] = '\0';
        line_char_reader_index++;
        index += 2; // to skip \r\n
        push(lines, line);
    }

    printf("---------\n");
    printf("%s\n", (char *)lines->data[0]);
    printf("---------\n");
    printf("%s\n", (char *)lines->data[1]);
    printf("---------\n");
    printf("%s\n", (char *)lines->data[2]);
    printf("---------\n");
    printf("%s\n", (char *)lines->data[3]);
    printf("---------\n");

    request_t *request = malloc(sizeof(request_t));
    char *line1 = (char *)lines->data[0];
    int line_index = 0;
    request->method = &line1[line_index];
    int uri_captured = 0;
    while (line1[line_index] != '\0')
    {
        if (uri_captured != 1 && line1[line_index] == ' ')
        {
            line1[line_index] = '\0';
            line_index++;
            request->uri = &line1[line_index];
            uri_captured = 1;
        }
        else if (uri_captured == 1 && line1[line_index] == ' ')
        {
            line1[line_index] = '\0';
            line_index++;
            request->protocol_version = &line1[line_index];
            break;
        }
        line_index++;
    }

    printf("%s\n", request->method);
    printf("%s\n", request->uri);
    printf("%s\n", request->protocol_version);

    send(new_socketfd, "Hello", sizeof("Hello"), 0);
    printf("sent: %s\n", "Hello");

    close(sockfd);
    return 0;
}