#include "response.h"
#include <string.h>
#include <stdio.h>

void serialize(response_t *response, char *result_str, size_t result_size)
{
    int body_len = strlen(response->body);
    snprintf(result_str, result_size, "%s %s %s\r\n"
                                      "Content-Type: %s\r\n"
                                      "Content-Length: %d\r\n"
                                      "Connection: Close\r\n"
                                      "\r\n"
                                      "%s",
             response->protocol_version, response->status_code, response->status_desc, response->content_type, body_len, response->body);
}