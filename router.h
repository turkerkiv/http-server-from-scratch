#include "arraylist.h"
#include "response.h"

typedef struct Router
{
    arraylist_t *routes;
} router_t;

typedef struct Route
{
    char *route_name;
    void (*function_name)(response_t *response);
} route_t;

void add_route(router_t *router, route_t *route);