#include "cpu.h"


std::map<byte,instruction> cpu::op_code_map = 
{
    
   


};


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
        switch(OPCODE)
        {
            case 0x00: BRK(); break;
            case 0x01: ORA(); break;
            case 0x05: ORA(); break;
            case 0x06: ASL(); break;
        }
        
        instruction info = op_code_map[OPCODE];

        
        wait_cycles = info.clock_cycles;
        PC += info.length; //increase PC

    }
}


byte cpu::find_value_by_mode(ADR adressing_mode)
{

    byte read;
    byte r1;
    byte_2 data_memory_adress;
    byte_2 intermediate_adress;

    switch(adressing_mode)
    {
        case ADR::ZP:
        read = _bus->read((byte_2)_bus->read(PC + 1));
        break;

        case ADR::ZPX:
        read = _bus->read((byte_2)_bus->read(PC + 1) + (byte_2)X);
        break;

        case ADR::ZPY:   
        read = _bus->read(((byte_2)_bus->read(PC + 1)) + (byte_2)Y);
        break;

        case ADR::ABS:
        read = _bus->read(((byte_2)_bus->read(PC + 1) + 256 * (byte_2)_bus->read(PC + 2)));
        break;

        case ADR::ABSX:
        r1 = _bus->read((byte_2)_bus->read(PC+1) + X);
        if(r1 > 0x00FF) extra_cycle = true;
        read = _bus->read(r1 + (byte_2)_bus->read(PC+2)*256);
        break;

        case ADR::ABSY:
        r1 = _bus->read((byte_2)_bus->read(PC+1) + Y);
        if(r1 > 0x00FF) extra_cycle = true;
        read = _bus->read(r1 + (byte_2)_bus->read(PC+2)*256);
        break;

        case ADR::IND:
        data_memory_adress = _bus->read( _bus->read(PC+2) * 256 + byte_2(_bus->read(PC+1)));
        read = _bus->read(_bus->read(data_memory_adress) * 256 + _bus->read(data_memory_adress + 1));
        break;

        case ADR::IMM:
        read = _bus->read(PC + 1);
        break;

        case ADR::INDX:
        data_memory_adress = (byte_2)_bus->read(PC + 1) + (byte_2)X;
        read = _bus->read(256 * _bus->read(data_memory_adress) + data_memory_adress + Y);
        break;

        case ADR::INDY:
        data_memory_adress = (byte_2)_bus->read(PC+1);
        intermediate_adress = _bus->read(data_memory_adress) + Y;\
        if(intermediate_adress > 0x00FF) extra_cycle = true;
        read = _bus->read(intermediate_adress + 256 * _bus->read(data_memory_adress + 1));
        break;
    }
    return read;
}

//INSTRUCTIONS FUNCTIONS

void cpu::ORA()
{
    instruction info = op_code_map[OPCODE];
    byte oper = find_value_by_mode(info.mode);
    A = A | oper;
    //update flags N,Z
    if(A & 0x80 == 0x80) P = P | 0x80; //negative
    if(A == 0X00) P = P | 0x02; //zero
    

}

void cpu::NOP()
{
    
}

void cpu::BRK()
{
    
}

void cpu::ASL()
{
    
}