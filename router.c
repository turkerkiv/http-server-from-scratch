#include "router.h"
#include "arraylist.h"
#include "controller/error_controller.c"
#include "controller/home_controller.c"
#include "controller/user_controller.c"
#include <stdlib.h>
#include <string.h>
#include "request.h"

router_t *new_router()
{
    route_t *route = malloc(sizeof(route_t));
    route->route_name = "/users";
    route->method = "GET";
    route->handler_func = &handle_get_users;

    route_t *route3 = malloc(sizeof(route_t));
    route3->route_name = "/user";
    route3->method = "GET";
    route3->handler_func = &handle_get_user;

    route_t *route2 = malloc(sizeof(route_t));
    route2->route_name = "/";
    route2->method = "GET";
    route2->handler_func = &handle_index;

    route_t *route4 = malloc(sizeof(route_t));
    route4->route_name = "/api/hello";
    route4->method = "POST";
    route4->handler_func = &handle_api_hello;

    router_t *router = malloc(sizeof(router_t));
    router->routes_dispatch_table = new_arraylist(4);
    add_route(router, route);
    add_route(router, route2);
    add_route(router, route3);
    add_route(router, route4);
    return router;
}

void handle_route(router_t *router, request_t *request, response_t *response)
{
    int route_found = 0;
    char *uri = request->uri;
    char *method = request->method;
    for (int i = 0; i < router->routes_dispatch_table->count; i++)
    {
        route_t *route = (route_t *)router->routes_dispatch_table->data[i];
        // need one more check according to extension.
        // istek türüne göre getlerde eğer uri / veya .html ile bitiyorsa content type text/html oluyor bunda ve plain ise charset de eklenecek. yoksa json istiyordur. ya da bunun yolu accept headerına bakmaktır. bi de tabii status durumları da error alıp almamaya göre değişecek. sonra body yine istenilen şeye göre değişecek. şimdilik bunlar dinamik işte.

        if (strcmp(route->route_name, uri) == 0 && strcmp(route->method, method) == 0)
        {
            route->handler_func(request, response);
            route_found = 1;
            break;
        }
    }

    if (route_found == 0)
    {
        handle_not_found(request, response);
    }
}

void add_route(router_t *router, route_t *route)
{
    push(router->routes_dispatch_table, route);
}
