#include "entity.h"


struct Entity *entity_alloc(Vec2f pos)
{
    struct Entity *e = malloc(sizeof(struct Entity));
    e->pos = pos;

    return e;
}


void entity_free(struct Entity *e)
{
    free(e);
}

