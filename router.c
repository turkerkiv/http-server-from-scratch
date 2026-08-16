#include "router.h"
#include "arraylist.h"

void add_route(router_t *router, route_t *route)
{
    push(router->routes, route);
}
