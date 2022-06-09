#include "prog.h"
#include "util.h"
#include <time.h>
#include <SDL2/SDL_image.h>


struct Prog *prog_alloc(SDL_Window *w, SDL_Renderer *r)
{
    struct Prog *p = malloc(sizeof(struct Prog));
    p->running = true;
    p->restart = false;
    p->alive = true;

    p->window = w;
    p->rend = r;

    p->font = TTF_OpenFont("res/font.ttf", 16);

    p->player = player_alloc((Vec2f){ 19 * 64 + 32, 22 * 64 + 32 }, 0.f);
    p->map = map_alloc("map", "info");

    p->p_target_rot = 0.f;

    p->rotate_queue = 0.f;

    p->pellets = 0;
    p->npellets = 0;

    p->score = 0;

    for (int y = 0; y < p->map->dim.y; ++y)
    {
        for (int x = 0; x < p->map->dim.x; ++x)
        {
            if (p->map->layout[y * p->map->dim.x + x] == '.')
            {
                p->pellets = realloc(p->pellets, sizeof(struct Entity*) * ++p->npellets);
                p->pellets[p->npellets - 1] = entity_alloc((Vec2f){ x * 64 + 32, y * 64 + 32 });
            }
        }
    }

    p->total_pellets = p->npellets;

    for (int i = 0; i < 4; ++i)
    {
        p->ghosts[i] = entity_alloc((Vec2f){ 17 * 64 + 32 + 64 * i, 16 * 64 + 32 });
    }

    SDL_Point p0[] = {
        {18, 17},
        {20, 17},
        {20, 14},
        {27, 14},
        {27, 10},
        {30, 10},
        {30, 7},
        {2, 7},
        {2, 2},
        {10, 2},
        {10, 13},
        {5, 13},
        {5, 14},
        {2, 14},
        {2, 10},
        {5, 10},
        {5, 11},
        {10, 11},
        {10, 16},
        {13, 16},
        {13, 14},
        {20, 14},
        {20, 17}
    };

    for (size_t i = 0; i < sizeof(p0) / sizeof(p0[0]); ++i)
        p0[i] = (SDL_Point){ p0[i].x - 1, p0[i].y - 1 };

    SDL_Point p1[] = {
        {19, 17},
        {20, 17},
        {20, 14},
        {13, 14},
        {13, 10},
        {10, 10},
        {10, 7},
        {30, 7},
        {30, 2},
        {38, 2},
        {21, 2},
        {21, 7},
        {38, 7},
        {38, 14},
        {35, 14},
        {35, 13},
        {30, 13},
        {30, 16},
        {27, 16},
        {27, 20},
        {24, 20},
        {24, 23},
        {16, 23},
        {16, 20},
        {20, 20},
        {20, 17}
    };

    for (size_t i = 0; i < sizeof(p1) / sizeof(p1[0]); ++i)
        p1[i] = (SDL_Point){ p1[i].x - 1, p1[i].y - 1 };

    SDL_Point p2[] = {
        {20, 17},
        {20, 20},
        {24, 20},
        {24, 26},
        {20, 26},
        {20, 23},
        {16, 23},
        {16, 26},
        {2, 26},
        {2, 29},
        {7, 29},
        {7, 39},
        {2, 39},
        {2, 32},
        {2, 39},
        {18, 39},
        {18, 36},
        {13, 36},
        {13, 29},
        {18, 29},
        {18, 33},
        {27, 33},
        {27, 26},
        {24, 26},
        {24, 14},
        {20, 14}
    };

    for (size_t i = 0; i < sizeof(p2) / sizeof(p2[0]); ++i)
        p2[i] = (SDL_Point){ p2[i].x - 1, p2[i].y - 1 };

    SDL_Point p3[] = {
        {21, 17},
        {20, 17},
        {20, 20},
        {27, 20},
        {27, 34},
        {30, 34},
        {30, 26},
        {38, 26},
        {38, 29},
        {33, 29},
        {33, 32},
        {38, 32},
        {38, 39},
        {22, 39},
        {22, 36},
        {27, 36},
        {27, 20},
        {20, 20},
        {20, 17}
    };

    for (size_t i = 0; i < sizeof(p3) / sizeof(p3[0]); ++i)
        p3[i] = (SDL_Point){ p3[i].x - 1, p3[i].y - 1 };

    p->gpaths[0] = gp_alloc(p0, sizeof(p0) / sizeof(p0[0]));
    p->gpaths[1] = gp_alloc(p1, sizeof(p1) / sizeof(p1[1]));
    p->gpaths[2] = gp_alloc(p2, sizeof(p2) / sizeof(p2[2]));
    p->gpaths[3] = gp_alloc(p3, sizeof(p3) / sizeof(p3[3]));

    p->pellet_tex = IMG_LoadTexture(r, "res/pellet.png");
    p->ghost1_tex = IMG_LoadTexture(r, "res/ghost.png");
    p->ghost2_tex = IMG_LoadTexture(r, "res/ghost2.png");
    p->ghost_tex = p->ghost1_tex;

    clock_gettime(CLOCK_MONOTONIC, &p->ghost_animate_clock);

    return p;
}


void prog_free(struct Prog *p)
{
    SDL_DestroyTexture(p->pellet_tex);
    SDL_DestroyTexture(p->ghost1_tex);
    SDL_DestroyTexture(p->ghost2_tex);

    for (size_t i = 0; i < p->npellets; ++i)
        entity_free(p->pellets[i]);

    free(p->pellets);

    for (int i = 0; i < 4; ++i)
    {
        entity_free(p->ghosts[i]);
        gp_free(p->gpaths[i]);
    }

    player_free(p->player);
    map_free(p->map);

    TTF_CloseFont(p->font);
    free(p);
}


void prog_mainloop(struct Prog *p)
{
    SDL_Event evt;

    while (p->running)
    {
        while (SDL_PollEvent(&evt))
        {
            switch (evt.type)
            {
            case SDL_QUIT:
                p->running = false;
                break;
            case SDL_KEYDOWN:
            {
                switch (evt.key.keysym.sym)
                {
                case SDLK_LEFT:
                    p->rotate_queue = -M_PI / 2.f;
                    break;
                case SDLK_RIGHT:
                    p->rotate_queue = M_PI / 2.f;
                    break;
                case SDLK_DOWN:
                    p->rotate_queue = M_PI;
                    break;
                case SDLK_r:
                    if (!p->alive)
                    {
                        p->restart = true;
                        p->running = false;
                    }
                    break;
                }
            } break;
            }
        }

        if (p->alive)
            prog_handle_player(p);

        prog_move_ghosts(p);

        for (size_t i = 0; i < p->npellets; ++i)
        {
            if (vec_len(vec_subv(p->player->pos, p->pellets[i]->pos)) < 5.f)
            {
                ++p->score;
                entity_free(p->pellets[i]);
                memmove(p->pellets + i, p->pellets + i + 1, (--p->npellets - i) * sizeof(struct Entity*));

                if (p->npellets == 0)
                    p->alive = false;
            }
        }

        for (int i = 0; i < 4; ++i)
        {
            if (vec_len(vec_subv(p->player->pos, p->ghosts[i]->pos)) < 15.f)
            {
                p->alive = false;
            }
        }

        struct timespec now;
        clock_gettime(CLOCK_MONOTONIC, &now);

        if (util_timediff(&now, &p->ghost_animate_clock) >= .2f)
        {
            clock_gettime(CLOCK_MONOTONIC, &p->ghost_animate_clock);
            p->ghost_tex = p->ghost_tex == p->ghost1_tex ? p->ghost2_tex : p->ghost1_tex;
        }

#if 0
        printf("\n========\n");
        for (int i = 0; i < 4; ++i)
        {
            printf("Ghost %d: index %zu | target %d %d | current %d %d\n", i, p->gpaths[i]->idx, p->gpaths[i]->tile.x, p->gpaths[i]->tile.y, (int)p->ghosts[i]->pos.x / 64, (int)p->ghosts[i]->pos.y / 64);
        }
#endif

        SDL_RenderClear(p->rend);

        prog_render(p);

        if (!p->alive)
        {
            SDL_SetRenderDrawBlendMode(p->rend, SDL_BLENDMODE_BLEND);
            SDL_SetRenderDrawColor(p->rend, 0, 0, 0, 100);
            SDL_RenderFillRect(p->rend, 0);

            SDL_Texture *tex = util_render_text(p->rend, p->font, p->npellets == 0 ? "All pellets were collected, press [r] to restart" : "Press [r] to restart", (SDL_Color){ 255, 255, 255 });
            SDL_Rect r;
            SDL_QueryTexture(tex, 0, 0, &r.w, &r.h);
            r.x = 400 - r.w / 2;
            r.y = 400 - r.h / 2;

            SDL_RenderCopy(p->rend, tex, 0, &r);
            SDL_DestroyTexture(tex);
        }

        char s[20] = { 0 };
        sprintf(s, "Pellets: %d / %zu", p->score, p->total_pellets);
        SDL_Texture *score = util_render_text(p->rend, p->font, s, (SDL_Color){ 255, 255, 255 });
        SDL_Rect dst = { 20, 10 };
        SDL_QueryTexture(score, 0, 0, &dst.w, &dst.h);
        SDL_RenderCopy(p->rend, score, 0, &dst);
        SDL_DestroyTexture(score);

        SDL_SetRenderDrawColor(p->rend, 0, 0, 0, 255);
        SDL_RenderPresent(p->rend);
    }
}


void prog_handle_player(struct Prog *p)
{
    SDL_Point ipos = { (int)p->player->pos.x, (int)p->player->pos.y };

    if (p->rotate_queue && p->player->angle == p->p_target_rot)
    {
        float angle = p->player->angle + p->rotate_queue;
        SDL_Point index = {
            (p->player->pos.x + 64.f * cosf(angle)) / 64,
            (p->player->pos.y + 64.f * -sinf(angle)) / 64
        };

        if (p->map->layout[index.y * p->map->dim.x + index.x] != 'B')
        {
            bool rotate = false;

            SDL_Point tmp = ipos;
            int i;
            for (i = 0; i < 5; ++i)
            {
                tmp.x = ipos.x + i * (int)cosf(p->player->angle);
                tmp.y = ipos.y + i * (int)-sinf(p->player->angle);

                if (tmp.x % 32 == 0 && tmp.y % 32 == 0)
                {
                    // edge case
                    bool xe = (tmp.x + 32) % 64 != 0;
                    bool ye = (tmp.y + 32) % 64 != 0;

                    if (((int)sinf(angle) && xe) || ((int)cosf(angle) && ye))
                    {
                        if (((int)sinf(angle) && !xe) || ((int)cosf(angle) && !ye))
                        {
                            rotate = true;
                            break;
                        }
                    }
                    else
                    {
                        rotate = true;
                        break;
                    }
                }
            }

            if (rotate)
            {
                p->player->pos.x += i * (int)cosf(p->player->angle);
                p->player->pos.y += i * (int)-sinf(p->player->angle);

                p->p_target_rot = angle;
                p->rotate_queue = 0.f;
            }
        }
    }

    if (p->player->angle != p->p_target_rot)
    {
        p->player->angle += (p->p_target_rot - p->player->angle) / 5.f;

        if (fabsf(p->player->angle - p->p_target_rot) < .01f)
        {
            p->player->angle = p->p_target_rot;
        }
    }

    if (p->player->angle == p->p_target_rot)
    {
        p->player->pos = prog_move_pos(p, p->player->pos, (Vec2f){
            cosf(p->player->angle),
            sinf(p->player->angle)
        }, 0);
    }
}


void prog_move_ghosts(struct Prog *p)
{
    for (int i = 0; i < 4; ++i)
    {
        bool moved;
        struct GhostPath *gp = p->gpaths[i];
        SDL_Point diff = { gp->tile.x - gp->prev_tile.x, gp->tile.y - gp->prev_tile.y };

        int xsig = diff.x == 0 ? 0 : (diff.x < 0 ? -1 : 1);
        int ysig = diff.y == 0 ? 0 : (diff.y < 0 ? -1 : 1);

        p->ghosts[i]->pos = prog_move_pos(p, p->ghosts[i]->pos, (Vec2f){ xsig, -ysig }, &moved);

        float len = vec_len(vec_subv(p->ghosts[i]->pos, (Vec2f){ gp->tile.x * 64 + 32, gp->tile.y * 64 + 32 }));
        if (len < 6.f)
        {
            p->ghosts[i]->pos = (Vec2f){ gp->tile.x * 64 + 32, gp->tile.y * 64 + 32 };
            gp_next_idx(gp);
        }
    }
}


Vec2f prog_move_pos(struct Prog *p, Vec2f pos, Vec2f dir, bool *moved)
{
    SDL_Point collision = {
        pos.x + 32.f * dir.x,
        pos.y + 32.f * -dir.y
    };

    collision.x /= p->map->tile_size;
    collision.y /= p->map->tile_size;

    if (p->map->layout[collision.y * p->map->dim.x + collision.x] != 'B')
    {
        pos.x += 5.f * dir.x;
        pos.y -= 5.f * dir.y;

        if (moved) *moved = true;
    }
    else
    {
        int dx = (int)pos.x % 32;
        int dy = (int)pos.y % 32;

        if (dx < 32 - dx)
            pos.x -= dx;
        else
            pos.x += 32 - dx;

        if (dy < 32 - dy)
            pos.y -= dy;
        else
            pos.y += 32 - dy;

        if (moved) *moved = false;
    }

    return pos;
}


void prog_render(struct Prog *p)
{
    int x = 1;

    for (float i = p->player->angle - .463f; i < p->player->angle + .463f; i += .00115f)
    {
        SDL_Point endp = player_cast_ray(p->player, util_restrict_angle(i), p->map);
        SDL_Point delta = { endp.x - p->player->pos.x, endp.y - p->player->pos.y };

        float len = sqrtf(delta.x * delta.x + delta.y * delta.y);
        len *= cosf(util_restrict_angle(p->player->angle - i));

        float draw_height = p->map->tile_size * 800.f / len;
        float offset = 400.f - draw_height / 2.f;

        float brightness = fmax(0.f, 255.f - (len / 600.f) * 255.f);
        SDL_SetRenderDrawColor(p->rend, 0, 0, brightness, 255);
        SDL_RenderDrawLine(p->rend, x, offset, x, offset + draw_height);

        for (size_t j = 0; j < p->npellets; ++j)
        {
            if (vec_len(vec_subv(p->pellets[j]->pos, p->player->pos)) >= 600.f)
                continue;

            int col;
            float elen = player_cast_ray_entity(p->player, i, p->pellets[j], &col);
            elen *= cosf(util_restrict_angle(p->player->angle - i));

            if (elen < len)
            {
                SDL_SetRenderDrawColor(p->rend, 255, 0, 0, 255);
                SDL_Rect src = { col, 0, 1, 32 };
                SDL_Rect dst = { x, 400, 1, 32.f * 800.f / elen };
                SDL_RenderCopy(p->rend, p->pellet_tex, &src, &dst);
            }
        }

        float glens[4];
        int cols[4];

        for (int j = 0; j < 4; ++j)
        {
            int col;
            float elen = player_cast_ray_entity(p->player, i, p->ghosts[j], &col);
            elen *= cosf(util_restrict_angle(p->player->angle - i));

            glens[j] = elen;
            cols[j] = col;
        }

        for (int j = 0; j < 4; ++j)
        {
            for (int k = j; k < 4; ++k)
            {
                if (glens[k] > glens[j])
                {
                    float tmpf = glens[j];
                    glens[j] = glens[k];
                    glens[k] = tmpf;

                    int tmpi = cols[j];
                    cols[j] = cols[k];
                    cols[k] = tmpi;
                }
            }
        }

        for (int j = 0; j < 4; ++j)
        {
            if (glens[j] >= 600.f)
                continue;

            if (glens[j] < len)
            {
                SDL_SetRenderDrawColor(p->rend, 255, 0, 0, 255);
                SDL_Rect src = { cols[j], 0, 1, 32 };
                SDL_Rect dst = { x, 400, 1, 32.f * 800.f / glens[j] };
                SDL_RenderCopy(p->rend, p->ghost_tex, &src, &dst);
            }
        }

        ++x;
    }
}

