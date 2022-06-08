#ifndef PLAYER_H
#define PLAYER_H

#include "map.h"
#include "entity.h"
#include "vector.h"
#include <SDL2/SDL.h>

struct Player
{
    Vec2f pos;
    float angle;
};

struct Player *player_alloc(Vec2f pos, float angle);
void player_free(struct Player *p);

SDL_Point player_cast_ray(struct Player *p, float angle, struct Map *m);
SDL_Point player_cast_ray_horizontal(struct Player *p, float angle, struct Map *m);
SDL_Point player_cast_ray_vertical(struct Player *p, float angle, struct Map *m);

float player_cast_ray_entity(struct Player *p, float angle, struct Entity *e, int *col);

void player_move_forwards(struct Player *p, struct Map *m);

#endif

