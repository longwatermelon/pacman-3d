#ifndef UTIL_H
#define UTIL_H

#include "vector.h"
#include <SDL2/SDL_ttf.h>

char *util_read_file(const char *fp);

float util_restrict_angle(float angle);

// Returns distance along vector p with direction r
float util_rays_intersection(Vec2f p, Vec2f r, Vec2f q, Vec2f s);

SDL_Texture *util_render_text(SDL_Renderer* rend, TTF_Font* font, const char* text, SDL_Color color);

// t1 - t2
float util_timediff(struct timespec *t1, struct timespec *t2);

#endif

