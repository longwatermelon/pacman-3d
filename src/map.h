#ifndef MAP_H
#define MAP_H

#include <SDL2/SDL.h>

struct Map
{
    char *layout;
    SDL_Point dim;
    int tile_size;
};

struct Map *map_alloc(const char *map_fp, const char *info_fp);
void map_free(struct Map *m);

#endif

