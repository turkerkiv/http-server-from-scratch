#include "arraylist.h"
#include "response.h"

typedef struct Router
{
    arraylist_t *routes;
} router_t;

typedef struct Route
{
    char *route_name;
    void (*handler_func)(response_t *response);
} route_t;

router_t *new_router();
void add_route(router_t *router, route_t *route);
void handle_route(router_t *router, response_t *response, char *uri);