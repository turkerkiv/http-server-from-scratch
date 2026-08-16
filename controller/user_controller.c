#include "../response.h"
#include "../request.h"
#include <string.h>

void handle_get_users(request_t *request, response_t *response)
{
    strcpy(response->protocol_version, "HTTP/1.1");
    strcpy(response->status_desc, "OK");
    strcpy(response->status_code, "200");
    strcpy(response->content_type, "text/html; charset=UTF-8");
    strcpy(response->body, "<html><body><h1>Hello, World!</h1></body></html>");
}

void handle_get_user(request_t *request, response_t *response)
{
    strcpy(response->protocol_version, "HTTP/1.1");
    strcpy(response->status_desc, "OK");
    strcpy(response->status_code, "200");
    strcpy(response->content_type, "text/html; charset=UTF-8");
    strcpy(response->body, "<html><body><h1>The user is Türker</h1></body></html>");
}
