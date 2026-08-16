#include "router.h"
#include "arraylist.h"
#include "controller/home_controller.c"
#include "controller/error_controller.c"
#include "controller/user_controller.c"
#include <stdlib.h>
#include <string.h>

router_t *new_router()
{
    route_t *route = malloc(sizeof(route_t));
    route->route_name = "/users";
    route->handler_func = &handle_get_users;

    route_t *route3 = malloc(sizeof(route_t));
    route3->route_name = "/user";
    route3->handler_func = &handle_get_user;

    route_t *route2 = malloc(sizeof(route_t));
    route2->route_name = "/";
    route2->handler_func = &handle_index;

    route_t *route4 = malloc(sizeof(route_t));
    route4->route_name = "/api/hello";
    route4->handler_func = &handle_api_hello;

    router_t *router = malloc(sizeof(router_t));
    router->routes_dispatch_table = new_arraylist(4);
    add_route(router, route);
    add_route(router, route2);
    add_route(router, route3);
    add_route(router, route4);
    return router;
}

void handle_route(router_t *router, response_t *response, char *uri)
{
    int route_found = 0;
    for (int i = 0; i < router->routes_dispatch_table->count; i++)
    {
        route_t *route = (route_t *)router->routes_dispatch_table->data[i];
        if (strcmp(route->route_name, uri) == 0)
        {
            route->handler_func(response);
            route_found = 1;
            break;
        }
    }

    if (route_found == 0)
    {
        handle_not_found(response);
    }
}

void add_route(router_t *router, route_t *route)
{
    push(router->routes_dispatch_table, route);
}
