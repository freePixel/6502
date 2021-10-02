#include "cpu.h"

std::map<ADR , byte> cpu::instruction_size =
{
    {ADR::ACC , 1},
    {ADR::ABS , 3},
    {ADR::ABSX,3},
    {ADR::ABSY,3},
    {ADR::IMM,2},
    {ADR::IMP,1},
    {ADR::IND,3},
    {ADR::INDX,3},
    {ADR::INDY,3},
    {ADR::REL,2},
    {ADR::ZP,2},
    {ADR::ZPX,2},
    {ADR::ZPY,2}
};

std::map<byte,instruction> cpu::opcode_map =
{
    {0x69 , {IMM , 2}},{0x65,{ZP,3}},{0x75,{ZPX,4}},{0x6d,{ABS,4}},{0x7d,{ABSX,4}},{0x79,{ABSY,4}},{0x61,{INDX,6}},{0x71,{INDY,5}}  //ADC       



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
            case 0x69:
            case 0x65:
            case 0x75:
            case 0x6d:
            case 0x7d:
            case 0x79:
            case 0x61:
            case 0x71:
            ADC();
            break;
        }
        
        instruction info = opcode_map[OPCODE];
        wait_cycles = info.clock_cycles;
        PC += instruction_size[info.mode];

    }
}


byte cpu::find_operator_by_mode(ADR adressing_mode)
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

        case ADR::ACC:
        read = A;
        break;
    }
    return read;
}

//INSTRUCTIONS FUNCTIONS

void cpu::ADC()
{
    instruction info = opcode_map[OPCODE];
    byte oper = find_operator_by_mode(info.mode);
    A = A + oper + (P & 0x01);
    if(A & 0x80 == 0x80) P = P | 0x80; //negative
    if(A == 0x00) P = P | 0x02; //zero
    
}

void cpu::ORA()
{
    instruction info = opcode_map[OPCODE];
    byte oper = find_operator_by_mode(info.mode);
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