#include "prog.h"
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>


int main(int argc, char **argv)
{
    SDL_Init(SDL_INIT_VIDEO);
    IMG_Init(IMG_INIT_PNG);
    TTF_Init();

    SDL_Window *w = SDL_CreateWindow("Pacman 3d", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, 800, 800, SDL_WINDOW_SHOWN);
    SDL_Renderer *r = SDL_CreateRenderer(w, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);

    while (true)
    {
        struct Prog *p = prog_alloc(w, r);
        prog_mainloop(p);

        bool restart = p->restart;
        prog_free(p);

        if (!restart)
            break;
    }

    SDL_DestroyRenderer(r);
    SDL_DestroyWindow(w);

    TTF_Quit();
    IMG_Quit();
    SDL_Quit();

    return 0;
}

