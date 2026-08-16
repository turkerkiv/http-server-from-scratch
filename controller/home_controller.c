#include "../response.h"
#include "../request.h"
#include <string.h>
#include <stdio.h>

void handle_index(request_t *request, response_t *response)
{
    FILE *fptr;
    fptr = fopen("static/index.html", "r");
    if (fptr == NULL)
    {
        // need more error options
        handle_not_found(request, response);
        return;
    }
    char file_content[4096];
    int ch;
    int i = 0;
    while ((ch = fgetc(fptr)) != EOF)
    {
        file_content[i] = (char)ch;
        i++;
    }
    file_content[i] = '\0';
    fclose(fptr);

    strcpy(response->protocol_version, "HTTP/1.1");
    strcpy(response->status_desc, "OK");
    strcpy(response->status_code, "200");
    strcpy(response->content_type, "text/html; charset=UTF-8");
    strcpy(response->body, file_content);
}

void handle_api_hello(request_t *request, response_t *response)
{
    strcpy(response->protocol_version, "HTTP/1.1");
    strcpy(response->status_desc, "OK");
    strcpy(response->status_code, "200");
    strcpy(response->content_type, "application/json; charset=UTF-8");
    strcpy(response->body, "{\"message\": \"Hello from API!\"}");
}