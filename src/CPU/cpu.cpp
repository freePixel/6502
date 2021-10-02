#include "cpu.h"

cpu::cpu(bus* _bus)
{
    //set all registers to 0
    this->_bus = _bus; //connect bus to 6502
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
    if(wait_cycles > 0)
    {
        wait_cycles--;
        return;
    }
    if(extra_cycle) extra_cycle = false;
    else{
        //execute intructio
        OPCODE = _bus->read(PC);
        if(OPCODE == 0x00) BRK();
        if(OPCODE == 0x01) ORA();
        
    }
}


byte cpu::find_value_by_mode(ADR adressing_mode)
{
    switch(adressing_mode)
    {
        case ADR::ZP:
        byte_2 return_adress =  _bus->read((byte_2)_bus->read(PC + 1));
        PC += 2;
        return return_adress;

        case ADR::ZPX:
        byte_2 return_adress =  _bus->read((byte_2)_bus->read(PC + 1)) + (byte_2)X);
        PC += 2;
        return return_adress;
        case ADR::ZPY:
        
        byte_2 return_adress =  _bus->read(((byte_2)_bus->read(PC + 1)) + (byte_2)Y);
        PC += 2;
        return return_adress;

        case ADR::ABS:
        byte_2 return_adress = _bus->read(((byte_2)_bus->read(PC + 1) + 256 * (byte_2)_bus->read(PC + 2)));
        PC += 3;
        return return_adress;

        case ADR::ABSX:
        byte_2 r1 = _bus->read((byte_2)_bus->read(PC+1) + X);
        if(r1 > 0x00FF) extra_cycle = true;
        byte_2 return_adress =  _bus->read(r1 + (byte_2)_bus->read(PC+2)*256);
        PC += 3;
        return return_adress;

        case ADR::ABSY:
        byte_2 r1 = _bus->read((byte_2)_bus->read(PC+1) + Y);
        if(r1 > 0x00FF) extra_cycle = true;
        byte_2 return_value = _bus->read(r1 + (byte_2)_bus->read(PC+2)*256);
        PC += 3;
        return return_value;

        case ADR::IND:
        byte_2 data_memory_adress = _bus->read( _bus->read(PC+2) * 256 + byte_2(_bus->read(PC+1)));
        PC += 3;
        return _bus->read(_bus->read(data_memory_adress) * 256 + _bus->read(data_memory_adress + 1));

        case ADR::IMM:
        PC += 2;
        return _bus->read(PC + 1);

        case ADR::INDX:
        byte_2 data_memory_adress = (byte_2)_bus->read(PC + 1) + (byte_2)X;
        PC += 2;
        return _bus->read(256 * _bus->read(data_memory_adress) + data_memory_adress + Y);

        case ADR::INDY:
        
        byte_2 data_memory_adress = (byte_2)_bus->read(PC+1);
        byte_2 intermediate_adress = _bus->read(data_memory_adress) + Y;
        if(intermediate_adress > 0x00FF) extra_cycle = true;
        PC += 2;
        return _bus->read(intermediate_adress + 256 * _bus->read(data_memory_adress + 1));
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
        wait_cycles = 2;
        break;

        case 0x05:
        MODE = ADR::ZP;
        wait_cycles = 3;
        break;

        case 0x15:
        MODE = ADR::ZPX;
        wait_cycles = 4;
        break;

        case 0x0d:
        MODE = ADR::ABS;
        wait_cycles = 4;
        break;

        case 0x1d:
        MODE = ADR::ABSX;
        wait_cycles = 4;
        break;

        case 0x19:
        MODE = ADR::ABSY;
        wait_cycles = 4;
        break;

        case 0x01:
        MODE = ADR::INDX;
        wait_cycles = 6;
        break;

        case 0x11:
        MODE = ADR::INDY;
        wait_cycles = 5;
        break;
    }

    byte oper = find_value_by_mode(MODE);
    A = A | oper;

    //update flags N,Z
    if(A & 0x80 == 0x80) P = P | 0x80; //negative
    if(A == 0X00) P = P | 0x02; //zero
    

}

void cpu::NOP()
{
    wait_cycles = 2;
    PC++;
}