#include <stddef.h>

typedef struct Response
{
    char protocol_version[16];
    char status_code[4];
    char status_desc[32];
    char content_type[32];
    char body[2048];
} response_t;

void serialize_response(response_t *response, char *result_str, size_t result_size);