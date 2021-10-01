#include "cpu.h"

cpu::cpu(bus* _bus)
{
    //set all registers to 0
    this->_bus = _bus;
    A = 0;
    X = 0;
    Y = 0;
    PC = 0;
    S = 0;
    P = 0;
    wait_cycles = 0;
}



void cpu::rising_edge_clk()
{
    if(wait_cycles > 0) wait_cycles--;
    else{
        //execute intructio
        OPCODE = _bus->read(PC);
        if(OPCODE == 0x00) BRK();
        if(OPCODE == 0x01) ORA();
        
    }
}


//INSTRUCTIONS FUNCTIONS

void cpu::ORA()
{
    ADR MODE;
    switch(OPCODE)
    {
        case 0x09:
        MODE = ADR::IMM;
        PC += 2;
        wait_cycles = 2;
        break;
        case 0x05:
        A = A | _bus->
    }
}
