#include <arpa/inet.h>
#include <stdio.h>
#include <string.h>
#include <sys/socket.h>
#include <unistd.h>

#define PORT 8080
#define SERVER_IP "127.0.0.1"

int main(void)
{
    int client_fd;
    struct sockaddr_in serv_addr;

    // Curl isteğindeki JSON gövdesi (body)
    char *json_body = "{\"key\": \"value\"}";
    int body_length = strlen(json_body);

    // HTTP POST İsteği Oluşturma
    // Not: \r\n HTTP protokolü için zorunludur.
    char request[2048];
    snprintf(request, sizeof(request),
             "POST /api/hello?param1=value1&param2=value2 HTTP/1.1\r\n"
             "Host: localhost:8080\r\n"
             "Content-Type: application/json\r\n"
             "Content-Length: %d\r\n"
             "Connection: close\r\n"
             "\r\n"
             "%s",
             body_length, json_body);

    char buffer[4096] = {0};

    client_fd = socket(AF_INET, SOCK_STREAM, 0);
    if (client_fd < 0)
    {
        perror("socket");
        return 1;
    }

    memset(&serv_addr, 0, sizeof(serv_addr));

    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(PORT);

    if (inet_pton(AF_INET, SERVER_IP, &serv_addr.sin_addr) <= 0)
    {
        perror("inet_pton");
        close(client_fd);
        return 1;
    }

    if (connect(client_fd,
                (struct sockaddr *)&serv_addr,
                sizeof(serv_addr)) < 0)
    {
        perror("connect");
        close(client_fd);
        return 1;
    }

    printf("Connected to server.\n");

    send(client_fd, request, strlen(request), 0);

    printf("Request sent:\n%s\n\n", request);

    int valread = recv(client_fd, buffer, sizeof(buffer) - 1, 0);

    if (valread > 0)
    {
        buffer[valread] = '\0';
        printf("Response received:\n%s\n", buffer);
    }

    printf("Closing connection.\n");
    close(client_fd);

    return 0;
}
