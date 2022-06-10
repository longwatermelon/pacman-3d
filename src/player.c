#include "player.h"
#include "util.h"
#include "vector.h"
#include <math.h>
#include <stdbool.h>
#include <limits.h>


struct Player *player_alloc(Vec2f pos, float angle)
{
    struct Player *p = malloc(sizeof(struct Player));
    p->pos = pos;
    p->angle = angle;

    return p;
}


void player_free(struct Player *p)
{
    free(p);
}


SDL_Point player_cast_ray(struct Player *p, float angle, struct Map *m)
{
    angle = util_restrict_angle(angle);

    SDL_Point h = player_cast_ray_horizontal(p, angle, m);
    SDL_Point v = player_cast_ray_vertical(p, angle, m);

    SDL_Point hdiff = { h.x - p->pos.x, h.y - p->pos.y };
    SDL_Point vdiff = { v.x - p->pos.x, v.y - p->pos.y };

    unsigned long hlen = sqrtf(hdiff.x * hdiff.x + hdiff.y * hdiff.y);
    unsigned long vlen = sqrtf(vdiff.x * vdiff.x + vdiff.y * vdiff.y);

    return hlen < vlen ? h : v;
}


SDL_Point player_cast_ray_horizontal(struct Player *p, float angle, struct Map *m)
{
    if (angle <= .001f || 2.f * M_PI - angle <= .001f)
        return (SDL_Point){ 1e5, p->pos.y };

    if (fabsf((float)M_PI - angle) <= .001f)
        return (SDL_Point){ -1e5, p->pos.y };

    SDL_FPoint nearest;
    nearest.y = (int)p->pos.y - ((int)p->pos.y % m->tile_size);

    if (angle > M_PI)
        nearest.y += m->tile_size;

    nearest.x = p->pos.x + ((nearest.y - p->pos.y) / -tanf(angle));

    while (true)
    {
        SDL_Point pos = {
            (nearest.x - fmod(nearest.x, m->tile_size)) / m->tile_size,
            (nearest.y - fmod(nearest.y, m->tile_size)) / m->tile_size
        };

        if (angle < M_PI)
            pos.y -= 1;

        if (pos.x < 0 || pos.x >= m->dim.x || pos.y < 0 || pos.y > m->dim.y)
            return (SDL_Point){ nearest.x, nearest.y };

        if (m->layout[pos.y * m->dim.x + pos.x] == 'B')
            return (SDL_Point){ nearest.x, nearest.y };

        float dy = (angle < M_PI ? -m->tile_size : m->tile_size);
        nearest.y += dy;
        nearest.x += dy / -tanf(angle);
    }
}


SDL_Point player_cast_ray_vertical(struct Player *p, float angle, struct Map *m)
{
    if (fabsf((float)(M_PI / 2.f) - angle) <= .001f)
        return (SDL_Point){ p->pos.x, -1e5 };

    if (fabsf((float)(M_PI * 3.f / 2.f) - angle) <= .001f)
        return (SDL_Point){ p->pos.x, 1e5 };

    SDL_FPoint nearest;
    nearest.x = (int)p->pos.x - ((int)p->pos.x % m->tile_size);

    if (angle < M_PI / 2.f || angle > 3.f * M_PI / 2.f)
        nearest.x += m->tile_size;

    nearest.y = p->pos.y + ((nearest.x - p->pos.x) * -tanf(angle));

    while (true)
    {
        SDL_Point pos = {
            (nearest.x - fmod(nearest.x, m->tile_size)) / m->tile_size,
            (nearest.y - fmod(nearest.y, m->tile_size)) / m->tile_size
        };

        if (angle > M_PI / 2.f && angle < 3.f * M_PI / 2.f)
            pos.x -= 1;

        if (pos.x < 0 || pos.x >= m->dim.x || pos.y < 0 || pos.y >= m->dim.y)
            return (SDL_Point){ nearest.x, nearest.y };

        if (m->layout[pos.y * m->dim.x + pos.x] == 'B')
            return (SDL_Point){ nearest.x, nearest.y };

        float dx = (angle < M_PI / 2.f || angle > 3.f * M_PI / 2.f ? m->tile_size : -m->tile_size);

        nearest.x += dx;
        nearest.y += dx * -tanf(angle);
    }
}


void player_move_forwards(struct Player *p, struct Map *m)
{
    float cos = cosf(p->angle);
    float sin = sinf(p->angle);

    SDL_Point collision = {
        p->pos.x + 32.f * cos,
        p->pos.y + 32.f * -sin
    };

    collision.x /= m->tile_size;
    collision.y /= m->tile_size;

    if (m->layout[collision.y * m->dim.x + collision.x] != 'B')
    {
        p->pos.x += 3.f * cos;
        p->pos.y -= 3.f * sin;
    }
    else
    {
        int dx = (int)p->pos.x % 32;
        int dy = (int)p->pos.y % 32;

        if (dx < 32 - dx)
            p->pos.x -= dx;
        else
            p->pos.x += 32 - dx;

        if (dy < 32 - dy)
            p->pos.y -= dy;
        else
            p->pos.y += 32 - dy;
    }
}


float player_cast_ray_entity(struct Player *p, float angle, struct Entity *e, int *col)
{
    float dist = INFINITY;
    *col = -1;

    Vec2f rdir = { cosf(angle), -sinf(angle) };
    Vec2f edir = { -sinf(p->angle), cosf(p->angle) };

    if (vec_dot(rdir, vec_normalize(vec_subv(e->pos, p->pos))) < 0.f)
        return dist;

    Vec2f end = vec_addv(e->pos, vec_mulf(edir, 16.f));
    int t = util_rays_intersection(end, vec_mulf(edir, -1.f), p->pos, rdir);

    if (t != -1)
    {
        dist = vec_len(vec_subv(vec_addv(end, vec_mulf(edir, -t)), p->pos));
        *col = (int)t;
    }

    return dist;
}

