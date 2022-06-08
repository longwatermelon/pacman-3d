#ifndef ENTITY_H
#define ENTITY_H

#include "vector.h"
#include <SDL2/SDL.h>

struct Entity
{
    Vec2f pos;
};

struct Entity *entity_alloc(Vec2f pos);
void entity_free(struct Entity *e);

#endif

