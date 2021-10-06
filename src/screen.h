#ifndef SCREEN_H
#define SCREEN_H

#include "cpu.h"

#include <SDL2/SDL.h>

class screen
{
    public:
        screen(cpu* CPU);
    private:

        SDL_Window* window = nullptr;
        SDL_Renderer* renderer = nullptr;

        cpu* CPU = nullptr;

};


#endif