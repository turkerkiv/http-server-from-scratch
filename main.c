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
#include <pthread.h>
#include "thread_dto.h"

#define PORT 8080

// create expects void * function
void *client_func(void *thread_dto)
{
    thread_dto_t *dto = (thread_dto_t *)thread_dto;
    int client_fd = dto->socketfd;
    router_t *router = dto->router;
    free(thread_dto);

    // request
    char request_buffer[1024] = {0};
    ssize_t valread = recv(client_fd, request_buffer, sizeof(request_buffer) - 1, 0);
    request_t *request = parse_to_request(request_buffer);

    // response
    response_t response;

    // router + response filling
    handle_route(router, request, &response);

    // sender
    char result_str[4096];
    serialize_response(&response, result_str, sizeof(result_str));
    send(client_fd, result_str, strlen(result_str), 0);

    // when each request finishes
    for (int i = 0; i < request->header_list->count; i++)
    {
        free(request->header_list->data[i]);
    }
    free(request->header_list);
    free(request);

    close(client_fd);
}

int main()
{
    struct sockaddr_in server_address;
    struct sockaddr_in client_address;
    socklen_t client_address_len = sizeof(client_address);
    router_t *router = new_router();

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

    while (1)
    {
        int new_socketfd = accept(sockfd, (struct sockaddr *)&client_address, &client_address_len);
        if (new_socketfd < 0)
        {
            perror("cannot accept");
            close(sockfd);
            exit(EXIT_FAILURE);
        }
        printf("Socket accepted client with a new descriptor: %d\n", new_socketfd);

        // create thread for async processes
        // (other alternatives are fork() and select())

        // malloc to get space because new_socketfd will become dangling in the next loop or compiler mallocs same space because of optimizing and race condition happens. (alternative is copy address and use it as void pointer)

        thread_dto_t *thread_dto = malloc(sizeof(thread_dto_t));
        thread_dto->socketfd = new_socketfd;
        thread_dto->router = router;
        pthread_t thread_id;
        if (pthread_create(&thread_id, NULL, client_func, (void *)thread_dto) < 0)
        {
            perror("thread could not create");
            free(thread_dto);
            close(new_socketfd);
            continue;
        }

        pthread_detach(thread_id);
    }

    close(sockfd);
    return 0;
}
