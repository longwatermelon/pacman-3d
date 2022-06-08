#ifndef GHOST_H
#define GHOST_H

#include "vector.h"
#include <SDL2/SDL.h>

struct GhostPath
{
    SDL_Point *tiles;
    size_t count;

    size_t idx;
    SDL_Point tile, prev_tile;
};

struct GhostPath *gp_alloc(SDL_Point *tiles, size_t count);
void gp_free(struct GhostPath *gp);

void gp_next_idx(struct GhostPath *gp);

#endif

