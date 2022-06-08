#include "ghost.h"


struct GhostPath *gp_alloc(SDL_Point *tiles, size_t count)
{
    struct GhostPath *gp = malloc(sizeof(struct GhostPath));

    gp->tiles = malloc(sizeof(SDL_Point) * count);
    memcpy(gp->tiles, tiles, sizeof(SDL_Point) * count);

    gp->count = count;
    gp->idx = 0;
    gp->tile = gp->tiles[0];
    gp_next_idx(gp);

    return gp;
}


void gp_free(struct GhostPath *gp)
{
    free(gp->tiles);
    free(gp);
}


void gp_next_idx(struct GhostPath *gp)
{
    ++gp->idx;

    if (gp->idx >= gp->count)
        gp->idx = 0;

    gp->prev_tile = gp->tile;
    gp->tile = gp->tiles[gp->idx];
}

