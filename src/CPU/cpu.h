#ifndef CPU_H
#define CPU_H

#include <array>
#include "../bus.h"

class cpu
{
    public:
    cpu(bus* bus);
    void rising_edge_clk();
    
    private:
        int wait_cycles; //wait cycles after executing an instruction

        //instructions
        
        void BRK(); void ORA(); void ADC(byte opcode);


        //registers
        byte A;         //accumulator
        byte X , Y;     //Indexes
        byte_2 PC;//program counter (2 byte wide)
        byte S; //stack pointer
        byte P; //status register, NEGATIVE | OVERFLOW | --- |  BREAK | DECIMAL | INTERRUPT-DISABLE | ZERO | CARRY

        bus* _bus;

};

#endif