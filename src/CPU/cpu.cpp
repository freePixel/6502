#include "cpu.h"

cpu::cpu()
{
    //set all registers to 0
    A = 0;
    X = 0;
    Y = 0;
    PC0 = 0;
    PC1 = 0;
    S = 0;
    P = 0;
    wait_cycles = 0;
}

void cpu::rising_edge_clk()
{
    if(wait_cycles > 0) wait_cycles--;
    else{
        //execute intruction
        
    }
}