#include "arraylist.h"
#include "request.h"
#include "request_header.h"
#include <stdlib.h>
#include <stdio.h>
#include "query_string.h"
#include <string.h>

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

    // extract query strings
    int uri_index = 0;
    int any_query_string = 1;
    while (new_request->uri[uri_index] != '?')
    {
        if (new_request->uri[uri_index] == '\0')
        {
            any_query_string = 0;
            break;
        }
        uri_index++;
    }
    if (any_query_string == 1)
    {
        new_request->uri[uri_index] = '\0'; // terminate uri before query string
        char *query_string_str = &new_request->uri[uri_index + 1];
        new_request->query_strings = extract_query_strings(query_string_str);
    }
    else
    {
        new_request->query_strings = new_arraylist(0);
    }

    printf("URI WITHOUT QUERY STRINGS: %s\n", new_request->uri);

    // extract headers
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

        printf("HEADER KEY%d: %s\n", i - 1, request_header->key);
        printf("HEADER VALUE%d: %s\n", i - 1, request_header->value);
    }

    // extract body
    char *body_line = (char *)lines->data[lines->count - 1];
    request_header_t *content_length_header;
    for (int i = 0; i < new_request->header_list->count; i++)
    {
        request_header_t *header = (request_header_t *)new_request->header_list->data[i];
        if (strcmp(header->key, "Content-Length") == 0)
        {
            content_length_header = header;
            break;
        }
    }
    new_request->body = body_line;
    int length = atoi(content_length_header->value);
    new_request->body[length] = '\0';
    printf("BODY: %s\n\n", new_request->body);

    // free lines but not data inside because they are still being used
    free(lines);
    return new_request;
}

arraylist_t *extract_query_strings(char *query_string_str)
{
    arraylist_t *query_strings = new_arraylist(2);
    int query_index = 0;
    query_string_t *current_query_string = malloc(sizeof(current_query_string));
    current_query_string->key = &query_string_str[query_index];
    while (query_string_str[query_index] != '\0')
    {
        if (query_string_str[query_index] == '=')
        {
            query_string_str[query_index] = '\0';
            query_index++;
            current_query_string->value = &query_string_str[query_index];
        }
        else if (query_string_str[query_index] == '&')
        {
            query_string_str[query_index] = '\0';

            push(query_strings, current_query_string);

            printf("QUERY STRING KEY%d: %s\n", query_strings->count - 1, current_query_string->key);
            printf("QUERY STRING VALUE%d: %s\n", query_strings->count - 1, current_query_string->value);

            current_query_string = malloc(sizeof(current_query_string));
            query_index++;
            current_query_string->key = &query_string_str[query_index];
        }
        else
        {
            query_index++;
        }
    }
    push(query_strings, current_query_string);
    printf("QUERY STRING KEY%d: %s\n", query_strings->count - 1, current_query_string->key);
    printf("QUERY STRING VALUE%d: %s\n", query_strings->count - 1, current_query_string->value);

    return query_strings;
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