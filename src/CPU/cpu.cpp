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
        byte instruction;
        switch(instruction)
        {
            case 0x00:
            BRK();
            break;
            case 0x01:
            ADC(instruction);
            break;
        }
        
    }
}


//INSTRUCTIONS FUNCTIONS

void ADC(byte instruction)
{
    switch(instruction)
    {
        case 0x69: //immediate mode
        
        break;
    }
}