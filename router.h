#include "arraylist.h"

typedef struct Router
{
    arraylist_t *routes;
} router_t;

typedef struct Route
{
    char *route_name;
    char *function_name;
} route_t;

void add_route(router_t *router, route_t *route);