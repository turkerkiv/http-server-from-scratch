#include "arraylist.h"
#include "response.h"
#include "request.h"

typedef struct Router
{
    arraylist_t *routes_dispatch_table;
} router_t;

typedef struct Route
{
    char *route_name;
    char *method;
    void (*handler_func)(request_t *request, response_t *response);
} route_t;

router_t *new_router();
void add_route(router_t *router, route_t *route);
void handle_route(router_t *router, request_t *request, response_t *response);