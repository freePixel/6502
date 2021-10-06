#include "screen.h"


screen::screen(cpu* CPU)
{
    this->window = SDL_CreateWindow("NES-emulator" , 256 , 240 , SDL_WINDOWPOS_CENTERED,SDL_WINDOWPOS_CENTERED,0);
    this->renderer = SDL_CreateRenderer(window , -1 , 0);
    this->CPU = CPU;
}

void screen::update()
{
    
}