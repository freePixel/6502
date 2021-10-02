#ifndef CPU_H
#define CPU_H

#include <array>
#include "bus.h"
#include <map>

class cpu
{
    public:
    cpu(bus* bus);
    void rising_edge_clk();
    
    private:

        static std::map<byte , instruction> opcode_map; //dictionary used to store all opcode info
        static std::map<ADR , byte> instruction_size;

        int wait_cycles; //wait cycles after executing an instruction
        bool extra_cycle = false; // if page boundary is crossed (X + RAM(PC+1) > 0xFF)

        byte find_operator_by_mode(ADR adressing_mode); // WARNING!: this function will update PC and wait_cloks variables
        //!!! INSTRUCTIONS !!!
        //there are 151  official opcodes
        void AND();
        void BRK();
        void ORA();
        void NOP();
        void ASL();
        void ADC();

        byte OPCODE; //current opcode being processed
        bool increase_pc;

        //registers
        byte A;         //accumulator
        byte X , Y;     //Indexes
        byte_2 PC;//program counter (2 byte wide)
        byte S; //stack pointer
        byte P; //status register, NEGATIVE | OVERFLOW | --- |  BREAK | DECIMAL | INTERRUPT-DISABLE | ZERO | CARRY

        bus* _bus;

};

#endif