#include <iostream>
#include "cpu.h"
#include <thread>
#include "definitions.h"
#include "ppu.h"
#include "screen.h"

int main(int argc, char* argv[])
{
    bus* BUS = new bus();
    cpu* CPU = new cpu(BUS);
    ppu* PPU = new ppu(BUS);
    screen* SCREEN = new screen(CPU);
    SDL_Event* e = new SDL_Event();
    bool update_cpu = false;
    while(true)
    {
        if(update_cpu)
        {
             CPU->rising_edge_clk();
             update_cpu = false;
        }
        SCREEN->update();
        std::this_thread::sleep_for(std::chrono::milliseconds(1000 / SCREEN_FPS));
        
        while(SDL_PollEvent(e) != 0)
        {
            if(e->type == SDL_QUIT)
            {
                SDL_Quit();
                return 0;
            }
            if(e->type == SDL_KEYDOWN)
            {
                update_cpu = true;
            }
        }
    }

    delete BUS;
    delete SCREEN;
    delete PPU;
    delete CPU;
}