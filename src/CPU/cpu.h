#ifndef CPU_H
#define CPU_H

#include <array>

typedef unsigned char byte;

class cpu
{
    public:
    cpu();
    void rising_edge_clk();
    private:
        
        int wait_cycles; //wait cycles after executing an instruction

        //instructions
        
        void BRK(); void ORA();


        //registers
        byte A;         //accumulator
        byte X , Y;     //Indexes
        byte PC1 , PC0; //program counter (2 byte wide)
        byte S; //stack pointer
        byte P; //status register, NEGATIVE | OVERFLOW | --- |  BREAK | DECIMAL | INTERRUPT-DISABLE | ZERO | CARRY

};

#endif