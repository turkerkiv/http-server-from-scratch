typedef struct Request
{
    char *method;
    char *uri;
    char *protocol_version;
    arraylist_t *header_list;
} request_t;