#ifndef PROG_H
#define PROG_H

#include "player.h"
#include "map.h"
#include "entity.h"
#include "ghost.h"
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

    struct Entity **pellets;
    size_t npellets;

    struct Entity *ghosts[4];
    struct GhostPath *gpaths[4];

    SDL_Texture *pellet_tex;
    SDL_Texture *ghost_tex;

    int score;
};

struct Prog *prog_alloc(SDL_Window *w, SDL_Renderer *r);
void prog_free(struct Prog *p);

void prog_mainloop(struct Prog *p);
void prog_handle_player(struct Prog *p);
void prog_move_ghosts(struct Prog *p);

Vec2f prog_move_pos(struct Prog *p, Vec2f pos, Vec2f dir, bool *moved);

void prog_render(struct Prog *p);

#endif

