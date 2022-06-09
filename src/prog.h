#ifndef PROG_H
#define PROG_H

#include "player.h"
#include "map.h"
#include "entity.h"
#include "ghost.h"
#include <stdbool.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>

struct Prog
{
    bool running;
    bool restart;
    bool alive;

    SDL_Window *window;
    SDL_Renderer *rend;

    TTF_Font *font;

    struct Player *player;
    struct Map *map;

    float p_target_rot;

    float rotate_queue;

    struct Entity **pellets;
    size_t npellets, total_pellets;

    struct Entity *ghosts[4];
    struct GhostPath *gpaths[4];

    SDL_Texture *pellet_tex;
    SDL_Texture *ghost_tex, *ghost1_tex, *ghost2_tex;
    struct timespec ghost_animate_clock;

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

