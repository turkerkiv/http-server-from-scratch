#include <sys/socket.h> // socket()
#include <stdio.h>      // input output
#include <stdlib.h>     // exit status code
#include <unistd.h>     // close()
#include <netinet/in.h> // sockaddr_in struct
#include "arraylist.h"
#include "request.h"
#include "request_header.h"
#include "response.h"
#include <string.h>
#include "router.h"
#define PORT 8080

int main()
{
    char buffer[1024] = {0};
    struct sockaddr_in server_address;
    struct sockaddr_in client_address;
    socklen_t client_address_len = sizeof(client_address);

    route_t *route = malloc(sizeof(route_t));
    route->route_name = "/hello";
    route->function_name = "hello_function";

    route_t *route2 = malloc(sizeof(route_t));
    route2->route_name = "/";
    route2->function_name = "root_function";

    router_t *router = malloc(sizeof(router_t));
    router->routes = new_arraylist(4);
    add_route(router, route);
    add_route(router, route2);

    int sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd < 0)
    {
        perror("Failed to open socket");
        exit(EXIT_FAILURE);
    };
    printf("Socket opened with a descriptor: %d\n", sockfd);

    int opt = 1;
    if (setsockopt(sockfd, SOL_SOCKET,
                   SO_REUSEADDR, &opt,
                   sizeof(opt)))
    {
        perror("setsockopt");
        exit(EXIT_FAILURE);
    }

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

    arraylist_t *lines = new_arraylist(4);

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
    char *first_line = (char *)lines->data[0];
    int line_index = 0;
    request->method = &first_line[line_index];
    int uri_captured = 0;
    while (first_line[line_index] != '\0')
    {
        if (uri_captured != 1 && first_line[line_index] == ' ')
        {
            first_line[line_index] = '\0';
            line_index++;
            request->uri = &first_line[line_index];
            uri_captured = 1;
        }
        else if (uri_captured == 1 && first_line[line_index] == ' ')
        {
            first_line[line_index] = '\0';
            line_index++;
            request->protocol_version = &first_line[line_index];
            break;
        }
        line_index++;
    }
    printf("%s\n", request->method);
    printf("%s\n", request->uri);
    printf("%s\n", request->protocol_version);

    request->header_list = new_arraylist(4);
    for (int i = 1; i < lines->count; i++)
    {
        char *line = (char *)lines->data[i];

        int header_line_index = 0;
        request_header_t *request_header = malloc(sizeof(request_header_t));
        request_header->key = &line[header_line_index];
        while (line[header_line_index] != '\0')
        {
            if (line[header_line_index] == ':' && line[header_line_index + 1] == ' ')
            {
                line[header_line_index] = '\0';
                header_line_index += 2;
                request_header->value = &line[header_line_index];
                break;
            }
            header_line_index++;
        }
        push(request->header_list, request_header);
        printf("%s\n", request_header->key);
        printf("%s\n", request_header->value);
    }

    // need to free lines() but not the values inside? or maybe use inside of it directly
    free(lines);

    // response part
    // istek türüne göre getlerde eğer uri / veya .html ile bitiyorsa content type text/html oluyor bunda ve plain ise charset de eklenecek. yoksa json istiyordur. ya da bunun yolu accept headerına bakmaktır. bi de tabii status durumları da error alıp almamaya göre değişecek. sonra body yine istenilen şeye göre değişecek. şimdilik bunlar dinamik işte.

    int route_found = 0;
    for (int i = 0; i < router->routes->count; i++)
    {
        route_t *route = (route_t *)router->routes->data[i];
        if (strcmp(route->route_name, request->uri) == 0)
        {
            // route->function_name;
            route_found = 1;
        }
    }

    if (route_found == 0)
    {
        response_t response;
        strcpy(response.protocol_version, "HTTP/1.1");
        strcpy(response.status_desc, "Not found");
        strcpy(response.status_code, "404");
        strcpy(response.content_type, "text/html; charset=UTF-8");
        strcpy(response.body, "<html><body><h1>No route found</h1></body></html>");
        int body_len = strlen(response.body);

        char result_str[4096];
        serialize(&response, result_str, sizeof(result_str));
        send(new_socketfd, result_str, strlen(result_str), 0);
    }
    else
    {
        response_t response;
        strcpy(response.protocol_version, "HTTP/1.1");
        strcpy(response.status_desc, "OK");
        strcpy(response.status_code, "200");
        strcpy(response.content_type, "text/html; charset=UTF-8");
        strcpy(response.body, "<html><body><h1>Hello, World!</h1></body></html>");
        int body_len = strlen(response.body);

        char result_str[4096];
        serialize(&response, result_str, sizeof(result_str));
        send(new_socketfd, result_str, strlen(result_str), 0);
    }

    close(sockfd);
    return 0;
}