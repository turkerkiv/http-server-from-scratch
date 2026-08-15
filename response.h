typedef struct Response
{
    char protocol_version[16];
    char status_code[4];
    char status_desc[32];
} response_t;