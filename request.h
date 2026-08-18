typedef struct Request
{
    char *method;
    char *uri;
    char *protocol_version;
    arraylist_t *header_list;
    char *body;
} request_t;

request_t *parse_to_request(char *request_str);
arraylist_t *break_into_lines(char *request_str);