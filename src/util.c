#include "util.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <math.h>


char *util_read_file(const char *path)
{
    char *contents = malloc(sizeof(char));
    contents[0] = '\0';

    FILE *fp = fopen(path, "r");
    char *line = 0;
    size_t len = 0;
    ssize_t read;

    if (!fp)
    {
        fprintf(stderr, "[util_read_file]: Couldn't read file %s\n", path);
        return 0;
    }

    while ((read = getline(&line, &len, fp)) != -1)
    {
        unsigned long prev_length = strlen(contents);
        contents = realloc(contents, (strlen(contents) + read) * sizeof(char));

        memcpy(&contents[prev_length], line, (read - 1) * sizeof(char));
        contents[prev_length + read - 1] = '\0';
    }

    free(line);
    fclose(fp);

    return contents;
}


float util_restrict_angle(float angle)
{
    while (angle < 0.f)
        angle += 2.f * M_PI;

    while (angle > 2.f * M_PI)
        angle -= 2.f * M_PI;

    return angle;
}


float util_rays_intersection(Vec2f p, Vec2f r, Vec2f q, Vec2f s)
{
    // p + tr = q + us
    // t = (q - p) X s / (r X s)

    float rxs = vec_cross(r, s);
    float t = vec_cross(vec_subv(q, p), s) / rxs;

    if (rxs != 0.f && t >= 0.f && t <= 32.f)
        return t;
    else
        return -1;
}


SDL_Texture *util_render_text(SDL_Renderer* rend, TTF_Font* font, const char* text, SDL_Color color)
{
    if (!text[0])
        return 0;

    SDL_Surface* surf = TTF_RenderUTF8_Blended(font, text, color);
    SDL_Texture* tex = SDL_CreateTextureFromSurface(rend, surf);

    SDL_FreeSurface(surf);

    return tex;
}


float util_timediff(struct timespec *t1, struct timespec *t2)
{
    return (t1->tv_sec - t2->tv_sec) + (t1->tv_nsec - t2->tv_nsec) / 1e9;
}

