#ifndef RAM_H
#define RAM_H

#include <array>
#include "definitions.h"

class ram
{
    public:
    ram();
    void write(byte_2 adress , byte data);
    byte read(byte_2 adress);
    private:
    std::array<byte , RAM_LA - RAM_FA + 1> memory;
};



#endif