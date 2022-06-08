#ifndef PROG_H
#define PROG_H

#include "player.h"
#include "map.h"
#include "entity.h"
#include <stdbool.h>
#include <SDL2/SDL.h>

struct Prog
{
    bool running;

    SDL_Window *window;
    SDL_Renderer *rend;

    struct Player *player;
    struct Map *map;

    float p_target_rot;

    float rotate_queue;

    struct Entity **entities;
    size_t nentities;
};

struct Prog *prog_alloc(SDL_Window *w, SDL_Renderer *r);
void prog_free(struct Prog *p);

void prog_mainloop(struct Prog *p);

void prog_render(struct Prog *p);

#endif

