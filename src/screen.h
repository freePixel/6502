#ifndef SCREEN_H
#define SCREEN_H

#include "cpu.h"
#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include <string>
#include <sstream>

class screen
{
    public:
        screen(cpu* CPU);
        void update();
    private:

        void draw_cpu();
        void draw_ppu();
    

        SDL_Texture* render_text(std::string text);

        SDL_Window* window = nullptr;
        SDL_Renderer* renderer = nullptr;
        cpu* CPU = nullptr;
        
        TTF_Font* font = nullptr;
        

};


#endif