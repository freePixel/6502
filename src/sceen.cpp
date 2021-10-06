#include "screen.h"


screen::screen(cpu* CPU)
{
    SDL_Init(SDL_INIT_EVERYTHING);
    window = SDL_CreateWindow("NES-Emulator" , SDL_WINDOWPOS_CENTERED , SDL_WINDOWPOS_CENTERED , 712,480,0);
    std::cout << SDL_GetError() << "\n";
    renderer = SDL_CreateRenderer(window , -1 , 0);
    this->CPU = CPU;
    TTF_Init();
    const char* file = "font/Fairfax-MJ0J.ttf";
    font = TTF_OpenFont(file , 24);

}

void screen::update()
{
    SDL_RenderClear(renderer);
    SDL_SetRenderDrawColor(renderer, 0,0,0,0);
    draw_cpu();
    draw_ppu();
    SDL_SetRenderDrawColor(renderer, 0,0,0,0);
    SDL_RenderPresent(renderer);
}

void screen::draw_cpu()
{   
    //render background
    SDL_SetRenderDrawColor(renderer , 0 , 0 , 255 , SDL_ALPHA_OPAQUE);
    SDL_FRect* temp_rect = new SDL_FRect{512 , 0 , 200 , 480};
    SDL_RenderFillRectF(renderer,temp_rect);
    delete temp_rect;

    std::array<int , 8> data = this->CPU->get_cpu_status();
    std::array<std::string , 8> st = {"A","X","Y","PC","S","P","OP","M[PC]"};
    for(int i=0;i<8;i++)
    {
        std::stringstream sstream;
        sstream << std::hex << data[i];
        std::string text = st[i] + ": " + sstream.str();
        SDL_Rect* r = new SDL_Rect{512+5,i*40 , 100 , 40};
        SDL_Texture* t = render_text(text);
        SDL_RenderCopy(renderer , t , NULL , r);
        delete r;
        SDL_DestroyTexture(t);

    }

    
    
}

void screen::draw_ppu()
{

}

SDL_Texture* screen::render_text(std::string text)
{
    SDL_Surface* surface = TTF_RenderText_Solid(font , text.c_str(), {255,255,200,0});
    SDL_Texture* texture = SDL_CreateTextureFromSurface(renderer , surface);
    SDL_FreeSurface(surface);
    
    return texture;
}