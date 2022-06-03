#include "prog.h"
#include "util.h"


struct Prog *prog_alloc(SDL_Window *w, SDL_Renderer *r)
{
    struct Prog *p = malloc(sizeof(struct Prog));
    p->running = true;

    p->window = w;
    p->rend = r;

    p->player = player_alloc((SDL_FPoint){ 1280 - 32, 1088 - 32 }, 0.f);
    p->map = map_alloc("map", "info");

    p->p_target_rot = 0.f;

    p->rotate_queue = 0.f;

    return p;
}


void prog_free(struct Prog *p)
{
    player_free(p->player);
    map_free(p->map);
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
                }
            } break;
            }
        }

        SDL_Point ipos = { (int)p->player->pos.x, (int)p->player->pos.y };

        if (p->rotate_queue && p->player->angle == p->p_target_rot)
        {
            float angle = p->player->angle + p->rotate_queue;
            SDL_Point index = {
                (p->player->pos.x + 64.f * cosf(angle)) / 64,
                (p->player->pos.y + 64.f * -sinf(angle)) / 64
            };

            if (p->map->layout[index.y * p->map->dim.x + index.x] == ' ')
            {
                bool rotate = false;

                SDL_Point tmp = ipos;
                int i;
                for (i = 0; i < 3; ++i)
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
            player_move_forwards(p->player, p->map);
        }

        SDL_RenderClear(p->rend);

        prog_render(p);

        SDL_SetRenderDrawColor(p->rend, 0, 0, 0, 255);
        SDL_RenderPresent(p->rend);
    }
}


void prog_render(struct Prog *p)
{
    int x = 1;

    for (float i = p->player->angle - M_PI / 6.f; i < p->player->angle + M_PI / 6.f; i += .0013f)
    {
        SDL_Point endp = player_cast_ray(p->player, util_restrict_angle(i), p->map);
        SDL_Point delta = { endp.x - p->player->pos.x, endp.y - p->player->pos.y };

        float len = sqrtf(delta.x * delta.x + delta.y * delta.y);
        len *= cosf(util_restrict_angle(p->player->angle - i));

        float draw_height = p->map->tile_size * 800.f / len;
        float offset = 400.f - draw_height / 2.f;

        SDL_SetRenderDrawColor(p->rend, 0, 0, 255, 255);
        SDL_RenderDrawLine(p->rend, x, offset, x, offset + draw_height);
        ++x;
    }
}

