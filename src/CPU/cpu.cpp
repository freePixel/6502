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
        PC += 2;
        return _bus->read((byte_2)_bus->read(PC + 1));

        case ADR::ZPX:
        PC += 2;
        return _bus->read((byte_2)_bus->read(PC + 1)) + (byte_2)X);
        case ADR::ZPY:
        PC += 2;
        return _bus->read(((byte_2)_bus->read(PC + 1)) + (byte_2)Y);

        case ADR::ABS:
        PC += 3;
        return _bus->read(((byte_2)_bus->read(PC + 1) + 256 * (byte_2)_bus->read(PC + 2)));

        case ADR::ABSX:
        PC += 3;
        byte_2 r1 = _bus->read((byte_2)_bus->read(PC+1) + X);
        if(r1 > 0x00FF) extra_cycle = true;
        return _bus->read(r1 + (byte_2)_bus->read(PC+2)*256);

        case ADR::ABSY:
        PC += 3;
        byte_2 r1 = _bus->read((byte_2)_bus->read(PC+1) + Y);
        if(r1 > 0x00FF) extra_cycle = true;
        return _bus->read(r1 + (byte_2)_bus->read(PC+2)*256);

        case ADR::IND:
        PC += 3;
        byte_2 data_memory_adress = _bus->read( _bus->read(PC+2) * 256 + byte_2(_bus->read(PC+1)));
        return _bus->read(_bus->read(data_memory_adress) * 256 + _bus->read(data_memory_adress + 1));

        case ADR::IMM:
        PC += 2;
        return _bus->read(PC + 1);

        case ADR::IXI:
        PC += 2;
        byte_2 data_memory_adress = (byte_2)_bus->read(PC + 1) + (byte_2)X;
        return final_adress = _bus->read(256 * _bus->read(data_memory_adress) + data_memory_adress + Y);

        case ADR::IXI:
        byte_2 data_memory_adress = (byte_2)_bus->read(PC+1);
        
        return _bus->read(256 * _bus->read(data_memory_adress + 1) + _bus->read(data_memory_adress) + Y);
    }
}

//INSTRUCTIONS FUNCTIONS

void cpu::ORA()
{
    switch(OPCODE)
    {
        case 0x09:
        A = A | _bus->read(PC + 1);
        PC += 2;
        wait_cycles = 2;
        break;
        case 0x05:
        A = A | _bus->read(_bus->read(PC+1));
        PC += 2;
        wait_cycles = 3;
        case 0x15:
        A = A | _bus->read(_bus->read(PC+1) + X);
        PC += 2;
        wait_cycles = 4;
        case 0x0d:
        A = A | _bus->read(_bus->read(PC+1) + _bus->read(PC+2) * 256);
        PC += 3;
        wait_cycles = 4;
        case 0x1d:
        A = A | _bus->read(_bus->read(PC+1) + _bus->read(PC+2) * 256 + X);
        if(_bus->read(PC+1) + X )
        break;
    }

}
