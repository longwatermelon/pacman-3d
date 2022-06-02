#include "prog.h"
#include "util.h"


struct Prog *prog_alloc(SDL_Window *w, SDL_Renderer *r)
{
    struct Prog *p = malloc(sizeof(struct Prog));
    p->running = true;

    p->window = w;
    p->rend = r;

    p->player = player_alloc((SDL_FPoint){ 1280, 1088 }, 0.f);
    p->map = map_alloc("map", "info");

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
            }
        }

        const Uint8* keys = SDL_GetKeyboardState(0);

        if (keys[SDL_SCANCODE_RIGHT]) p->player->angle += .1f;
        if (keys[SDL_SCANCODE_LEFT]) p->player->angle -= .1f;

        p->player->angle = util_restrict_angle(p->player->angle);

        if (keys[SDL_SCANCODE_W])
        {
            p->player->pos.x += 1.f * cosf(p->player->angle);
            p->player->pos.y += 1.f * -sinf(p->player->angle);
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

        SDL_SetRenderDrawColor(p->rend, 255, 0, 0, 255);
        SDL_RenderDrawLine(p->rend, x, offset, x, offset + draw_height);
        ++x;
    }
}

