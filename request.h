typedef struct Request
{
    char *method;
    char *uri;
    arraylist_t *query_strings;
    char *protocol_version;
    arraylist_t *header_list;
    char *body;
} request_t;

request_t *parse_to_request(char *request_str);
arraylist_t *extract_query_strings(char *uri);
arraylist_t *break_into_lines(char *request_str);