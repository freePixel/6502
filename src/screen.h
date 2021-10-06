#ifndef SCREEN_H
#define SCREEN_H

#include "cpu.h"
#include "SDL2/SDL_ttf.h"

class screen
{
    public:
        screen(cpu* CPU);
        void update();
    private:

        SDL_Window* window = nullptr;
        SDL_Renderer* renderer = nullptr;

        cpu* CPU = nullptr;
        

};


#endif