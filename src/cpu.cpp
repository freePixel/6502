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
    switch(adressing_mode)
    {
        case ADR::ZP:
        return _bus->read((byte_2)_bus->read(PC + 1));

        case ADR::ZPX:
        return _bus->read((byte_2)_bus->read(PC + 1) + (byte_2)X);

        case ADR::ZPY:   
        return _bus->read(((byte_2)_bus->read(PC + 1)) + (byte_2)Y);

        case ADR::ABS:
        return _bus->read(((byte_2)_bus->read(PC + 1) + 256 * (byte_2)_bus->read(PC + 2)));

        case ADR::ABSX:
        byte_2 r1 = _bus->read((byte_2)_bus->read(PC+1) + X);
        if(r1 > 0x00FF) extra_cycle = true;
        return _bus->read(r1 + (byte_2)_bus->read(PC+2)*256);

        case ADR::ABSY:
        byte_2 r2 = _bus->read((byte_2)_bus->read(PC+1) + Y);
        if(r2 > 0x00FF) extra_cycle = true;
        return _bus->read(r2 + (byte_2)_bus->read(PC+2)*256);

        case ADR::IND:
        byte_2 data_memory_adress = _bus->read( _bus->read(PC+2) * 256 + byte_2(_bus->read(PC+1)));
        return _bus->read(_bus->read(data_memory_adress) * 256 + _bus->read(data_memory_adress + 1));

        case ADR::IMM:
        return _bus->read(PC + 1);

        case ADR::INDX:
        byte_2 data_memory_adress = (byte_2)_bus->read(PC + 1) + (byte_2)X;
        return _bus->read(256 * _bus->read(data_memory_adress) + data_memory_adress + Y);

        case ADR::INDY:
        
        byte_2 data_memory_adress = (byte_2)_bus->read(PC+1);
        byte_2 intermediate_adress = _bus->read(data_memory_adress) + Y;
        if(intermediate_adress > 0x00FF) extra_cycle = true;
        return _bus->read(intermediate_adress + 256 * _bus->read(data_memory_adress + 1));
    }
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

void cpu::ASL()
{
    
}