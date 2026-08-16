#include "../response.h"

void handle_index(response_t *response)
{
    strcpy(response->protocol_version, "HTTP/1.1");
    strcpy(response->status_desc, "OK");
    strcpy(response->status_code, "200");
    strcpy(response->content_type, "text/html; charset=UTF-8");
    strcpy(response->body, "<html><body><h1>This is homepage</h1></body></html>");
}
