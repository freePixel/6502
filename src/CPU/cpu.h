#ifndef CPU_H
#define CPU_H

#include <array>

typedef unsigned char byte;

class cpu
{
    public:
    cpu();
    private:

        //registers
        byte A;         //accumulator
        byte X , Y;     //Indexes
        byte PC1 , PC0; //program counter (2 byte wide)
        byte S; //stack pointer
        byte P; //status register
};

#endif