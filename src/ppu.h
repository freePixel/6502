#ifndef PPU_H
#define PPU_H

#include <array>
#include "definitions.h"
#include "bus.h"

class ppu
{
    public:
        ppu(bus* _bus);
    private:

    //memory
    bus* _bus = nullptr;
};

#endif