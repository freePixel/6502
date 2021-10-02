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

        byte find_operator_by_mode(ADR adressing_mode); // WARNING!: this function will update PC and wait_cloks variables
        byte_2 find_adress_by_mode(ADR adressing_mode);

        void generate_NCZ_flags(byte enable_flag , byte_2 result);
        void generate_overflow_flag(byte OP1 , byte OP2);
        //!!! INSTRUCTIONS !!!
        //there are 151  official opcodes and 56 different instructions
        void ADC();
        void AND();
        void ASL();

        void branch_group(byte flag_bit, bool expected_value);
        void BCC(){branch_group(0, true);}
        void BCS(){branch_group(0, false);}
        void BEQ(){branch_group(1, true);}
        void BMI(){branch_group(7, true);}
        void BNE(){branch_group(1, false);}
        void BPL(){branch_group(7, false);}
        void BVC(){branch_group(6, false);}
        void BVS(){branch_group(6, true);}






        void BRK();
        void ORA();
        void NOP();

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