#include "arraylist.h"
#include "request.h"
#include "request_header.h"
#include <stdlib.h>
#include <stdio.h>

request_t *parse_to_request(char *request_str)
{
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

    printf("%s\n", new_request->method);
    printf("%s\n", new_request->uri);
    printf("%s\n", new_request->protocol_version);

    new_request->header_list = new_arraylist(4);
    for (int i = 1; i < lines->count; i++)
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

    // free lines but not data inside because they are still being used
    free(lines);
    return new_request;
}

arraylist_t *break_into_lines(char *request_str)
{
    arraylist_t *lines = new_arraylist(4);
    int index = 0;
    int line_char_reader_index = 0;
    while (!(request_str[index - 2] == '\r' && request_str[index - 1] == '\n' && request_str[index] == '\r' && request_str[index + 1] == '\n'))
    {
        int inner = 0;
        char *line = malloc(1024 * sizeof(char));
        while (request_str[index] != '\r')
        {
            line[inner] = request_str[index];
            inner++;
            index++;
        }
        line[inner] = '\0';
        line_char_reader_index++;
        index += 2; // to skip \r\n
        push(lines, line);
    }
    return lines;
}