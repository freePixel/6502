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
    {0x69 , {IMM , 2}},{0x65,{ZP,3}},{0x75,{ZPX,4}},{0x6d,{ABS,4}},{0x7d,{ABSX,4}},{0x79,{ABSY,4}},{0x61,{INDX,6}},{0x71,{INDY,5}},  //ADC
    {0x29,{IMM,2}},{0x25,{ZP,3}},{0x35,{ZPX,4}},{0x2d,{ABS,4}},{0x3d,{ABSX,4}},{0x39,{ABSY,4}},{0x21,{INDX,6}},{0x31,{INDY,5}},
    {0x0a,{ACC,2}},{0x06,{ZP,5}},{0x16,{ZPX,6}},{0x0e,{ABS,6}},{0x1e,{ABSX,7}},

    //branch instructions (relative)
    {0x90,{REL,2}},
    {0xb0,{REL,2}},
    {0xf0,{REL,2}},
    {0x30,{REL,2}},
    {0xd0,{REL,2}},
    {0x10,{REL,2}},
    {0x50,{REL,2}},
    {0x70,{REL,2}},

    {0x24 , {ZP,3}},{0x2c , {ABS,4}},
    {0x00 , {IMP,7}},
    {0x18,{IMP,2}},
    {0xb8,{IMP,2}},
    {0xd8,{IMP,2}},
    {0x58,{IMP,2}},

    {0xc9,{IMM,2}},{0xc5,{ZP,3}},{0xd5,{ZPX,4}},{0xcd,{ABS,4}},{0xdd,{ABSX,4}},{0xd9,{ABSY,4}},{0xc1,{INDX,6}},{0xd1,{INDY,5}}




};


cpu::cpu(bus* _bus)
{
    //set all registers to 0
    this->_bus = _bus; //connect bus to 6502
    A = 0;
    X = 0;
    Y = 0;
    PC = 0;
    S = 0xff;
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
    else{
        //execute intruction
        OPCODE = _bus->read(PC);
        switch(OPCODE)
        {
            case 0x69:case 0x65:case 0x75:case 0x6d:case 0x7d:case 0x79:case 0x61:case 0x71:ADC();break;
            case 0x29:case 0x25:case 0x35:case 0x2d:case 0x3d:case 0x39:case 0x21:case 0x31:AND();break;
            case 0x0a:case 0x06:case 0x16:case 0x0e:case 0x1e:ASL();break;
            case 0x90:BCC();break;
            case 0xb0:BCS();break;
            case 0xf0:BEQ();break;
            case 0x24:case 0x2c: BIT(); break;
            case 0x30:BMI();break;
            case 0xd0:BNE();break;
            case 0x10:BPL();break;
            case 0x00:BRK();break;
            case 0x50:BVC();break;
            case 0x70:BVS();break;
            case 0x18:CLC();break;
            case 0xd8:CLD();break;
            case 0x58:CLI();break;
            case 0xb8:CLV();break;

            
        }
        
        instruction info = opcode_map[OPCODE];
        wait_cycles += info.clock_cycles;
        PC += instruction_size[info.mode];

    }
}

byte cpu::find_operator_by_mode(ADR adressing_mode)
{
    if(adressing_mode == ADR::ACC)
    {
        return A;
    }
    else{
        return _bus->read(find_adress_by_mode(adressing_mode));
    }
}

byte_2 cpu::find_adress_by_mode(ADR adressing_mode)
{

    byte_2 read;
    byte r1;
    byte_2 data_memory_adress;
    byte_2 intermediate_adress;

    switch(adressing_mode)
    {
        case ADR::ZP:
        read = (byte_2)_bus->read(PC + 1);
        break;

        case ADR::ZPX:
        read = (byte_2)_bus->read(PC + 1) + (byte_2)X;
        break;

        case ADR::ZPY:   
        read = ((byte_2)_bus->read(PC + 1)) + (byte_2)Y;
        break;

        case ADR::ABS:
        read = ((byte_2)_bus->read(PC + 1) + 256 * (byte_2)_bus->read(PC + 2));
        break;

        case ADR::ABSX:
        r1 = _bus->read((byte_2)_bus->read(PC+1) + X);
        if(r1 > 0x00FF) wait_cycles += 1;
        read = r1 + (byte_2)_bus->read(PC+2)*256;
        break;

        case ADR::ABSY:
        r1 = _bus->read((byte_2)_bus->read(PC+1) + Y);
        if(r1 > 0x00FF) wait_cycles += 1;
        read = r1 + (byte_2)_bus->read(PC+2)*256;
        break;

        case ADR::IND:
        data_memory_adress = _bus->read( _bus->read(PC+2) * 256 + byte_2(_bus->read(PC+1)));
        read = _bus->read(data_memory_adress) * 256 + _bus->read(data_memory_adress + 1);
        break;

        case ADR::IMM:
        read = _bus->read(PC + 1);
        break;

        case ADR::INDX:
        data_memory_adress = (byte_2)_bus->read(PC + 1) + (byte_2)X;
        read = 256 * _bus->read(data_memory_adress) + data_memory_adress + Y;
        break;

        case ADR::INDY:
        data_memory_adress = (byte_2)_bus->read(PC+1);
        intermediate_adress = _bus->read(data_memory_adress) + Y;
        if(intermediate_adress > 0x00FF) wait_cycles += 1;
        read = intermediate_adress + 256 * _bus->read(data_memory_adress + 1);
        break;
    }
    return read;
}


void cpu::generate_NCZ_flags(byte enable_flag , byte_2 result)
{
    //check for c flag
    if(ACTIVE_BIT(enable_flag , 0))
    {
        if(ACTIVE_BIT(result , 8)) P |= 0x01;
        else{
            P &= 0xfe;
        }
    }
    //zero flag
    if(ACTIVE_BIT(enable_flag , 1))
    {
        if(result == 0x0000) P |= 0x10;
        else{
            P &= 0xfd;
        }
    }
    //negative flag
    if(ACTIVE_BIT(enable_flag , 7))
    {
        if(ACTIVE_BIT(result,7)) P |= 0x80;
        else{
            P &= 0x7f;
        }
    }
}

void cpu::generate_overflow_flag(byte OP1 , byte OP2)
{
    byte total = OP1 + OP2;
    if(total < OP1) P |= 0x40;
    else{
        P &= 0xbf;
    }
}

void cpu::push_stack(byte value)
{
    _bus->write((byte_2)0x100 + (byte_2)S , value);
    S--;
}

void cpu::pop_stack()
{
    S++;
}

//INSTRUCTIONS FUNCTIONS

void cpu::ADC()
{
    instruction info = opcode_map[OPCODE];
    byte oper = find_operator_by_mode(info.mode);
    byte carry = P & 0x01;
    byte_2 result = A + oper + carry;

    generate_overflow_flag(A , oper + 1); //warning!! possible bug
    generate_NCZ_flags(0x83 , result);
    A = (byte)(result % 256);
    
}

void cpu::AND()
{
    instruction info = opcode_map[OPCODE];
    byte oper = find_operator_by_mode(info.mode);
    A &= oper;
    generate_NCZ_flags(0x82 , (byte_2)A);
}

void cpu::ASL()
{
    instruction info = opcode_map[OPCODE];
    
    if(info.mode == ACC)
    {
        A = A << 1;
        generate_NCZ_flags(0x83 , (byte_2)A);
    }
    else{
        byte_2 adress = find_adress_by_mode(info.mode);
        byte oper = _bus->read(adress);
        oper = oper << 1;
        _bus->write(adress , oper);
        generate_NCZ_flags(0x83 , (byte_2)oper);
    }
    
}

void cpu::branch_group(byte flag_bit, bool expected_value)
{
    bool activated = ACTIVE_BIT(P , flag_bit);
    if(activated == expected_value)
    {
        byte oper = _bus->read(PC + 1);
        PC = PC + (signed char)oper + 2;
        if(((byte_2)PC & 0x00ff) + (signed char)oper + (byte_2)2 > 0x00ff) wait_cycles += 2;
    }
    else{
        PC += 2;
    }
}

void cpu::BIT()
{
    byte oper = find_operator_by_mode(opcode_map[this->OPCODE].mode);
    if(ACTIVE_BIT(oper , 7)) P |= 0x80;
    if(ACTIVE_BIT(oper , 6)) P |= 0x40;
    
    if(A && oper) P |= 0x02;
}

void cpu::BRK()
{

    byte_2  result = PC+2;
    push_stack((byte)(result >> 8));
    push_stack((byte)(result % 256));
    push_stack(P);

    PC = 0xfffe;
    P |= 0x04;
}


void cpu::CMP()
{
    byte oper = find_operator_by_mode(opcode_map[OPCODE].mode);

    byte_2 result = (byte_2)A - (byte_2)oper;
    generate_NCZ_flags(0x83,result);
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

