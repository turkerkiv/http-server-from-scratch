#include "../response.h"
#include "../request.h"
#include <string.h>

void handle_not_found(request_t *request, response_t *response)
{
    strcpy(response->protocol_version, "HTTP/1.1");
    strcpy(response->status_desc, "Not found");
    strcpy(response->status_code, "404");
    strcpy(response->content_type, "text/html; charset=UTF-8");
    strcpy(response->body, "<html><body><h1>No route found</h1></body></html>");
}
