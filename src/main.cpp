#include <iostream>
#include "cpu.h"
#include <thread>
#include "ppu.h"
#include "SDL2/SDL_ttf.h"


int main(int argc, char* argv[])
{
    bus* _bus = new bus();
    cpu* CPU = new cpu(_bus);
    ppu* PPU = new ppu(_bus);

    while(true)
    {
        CPU->rising_edge_clk();
        std::this_thread::sleep_for(std::chrono::seconds(1));
    }

    delete _bus;
    delete CPU;
}