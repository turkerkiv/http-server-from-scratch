#include "router.h"

typedef struct ThreadDto
{
    int socketfd;
    router_t *router;
} thread_dto_t;