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

        //!!! INSTRUCTIONS !!!
        //there are 151  official opcodes
        void BRK();
        void ORA();


        void read_from_bus(byte adressing_mode);

        byte OPCODE; //current opcode being processed

        //registers
        byte A;         //accumulator
        byte X , Y;     //Indexes
        byte_2 PC;//program counter (2 byte wide)
        byte S; //stack pointer
        byte P; //status register, NEGATIVE | OVERFLOW | --- |  BREAK | DECIMAL | INTERRUPT-DISABLE | ZERO | CARRY

        bus* _bus;

};

#endif