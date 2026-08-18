#include "arraylist.h"
#include "request.h"
#include "request_header.h"
#include <stdlib.h>
#include <stdio.h>

request_t *parse_to_request(char *request_str)
{
    // the last line is body with curly braces
    arraylist_t *lines = break_into_lines(request_str);

    request_t *new_request = malloc(sizeof(request_t));

    char *request_line = (char *)lines->data[0];
    int request_line_index = 0;
    new_request->method = &request_line[request_line_index];
    int uri_captured = 0;
    while (request_line[request_line_index] != '\0')
    {
        if (uri_captured != 1 && request_line[request_line_index] == ' ')
        {
            request_line[request_line_index] = '\0';
            request_line_index++;
            new_request->uri = &request_line[request_line_index];
            uri_captured = 1;
        }
        else if (uri_captured == 1 && request_line[request_line_index] == ' ')
        {
            request_line[request_line_index] = '\0';
            request_line_index++;
            new_request->protocol_version = &request_line[request_line_index];
            break;
        }
        request_line_index++;
    }

    printf("METHOD: %s\n", new_request->method);
    printf("URI: %s\n", new_request->uri);
    printf("PROTOCOL VERSION: %s\n", new_request->protocol_version);

    new_request->header_list = new_arraylist(4);
    for (int i = 1; i < lines->count - 1; i++)
    {
        char *header_line = (char *)lines->data[i];

        int header_line_index = 0;
        request_header_t *request_header = malloc(sizeof(request_header_t));
        request_header->key = &header_line[header_line_index];
        while (header_line[header_line_index] != '\0')
        {
            if (header_line[header_line_index] == ':' && header_line[header_line_index + 1] == ' ')
            {
                header_line[header_line_index] = '\0';
                header_line_index += 2;
                request_header->value = &header_line[header_line_index];
                break;
            }
            header_line_index++;
        }
        push(new_request->header_list, request_header);

        printf("KEY%d: %s\n", i, request_header->key);
        printf("VALUE%d: %s\n", i, request_header->value);
    }

    char *body_line = (char *)lines->data[lines->count - 1];
    new_request->body = body_line;
    printf("BODY: %s\n\n", new_request->body);

    // free lines but not data inside because they are still being used
    free(lines);
    return new_request;
}

arraylist_t *break_into_lines(char *request_str)
{
    arraylist_t *lines = new_arraylist(4);
    int global_index = 0;
    while (!(request_str[global_index - 2] == '\r' && request_str[global_index - 1] == '\n' && request_str[global_index] == '\r' && request_str[global_index + 1] == '\n'))
    {
        int line_index = 0;
        char *line = malloc(1024 * sizeof(char));
        while (request_str[global_index] != '\r')
        {
            line[line_index] = request_str[global_index];
            line_index++;
            global_index++;
        }
        line[line_index] = '\0';
        global_index += 2; // to skip \r\n
        push(lines, line);
    }

    global_index += 2; // to skip \r\n\r\n
    char *body = malloc(1024 * sizeof(char));
    body = &request_str[global_index];
    push(lines, body);

    return lines;
}