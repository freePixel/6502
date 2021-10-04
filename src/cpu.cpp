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

    {0xc9,{IMM,2}},{0xc5,{ZP,3}},{0xd5,{ZPX,4}},{0xcd,{ABS,4}},{0xdd,{ABSX,4}},{0xd9,{ABSY,4}},{0xc1,{INDX,6}},{0xd1,{INDY,5}},

    {0xe0,{IMM,2}},{0xe4,{ZP,3}},{0xec,{ABS,4}},
    {0xc0,{IMM,2}},{0xc4,{ZP,3}},{0xcc,{ABS,4}},

    {0xc6,{ZP,5}},{0xd6,{ZPX,6}},{0xce,{ABS,6}},{0xde,{ABSX,7}},
    {0xca,{IMP,2}},
    {0x88,{IMP,2}},

    {0x49,{IMM,2}},{0x45,{ZP,3}},{0x55,{ZPX,4}},{0x4d,{ABS,4}},{0x5d,{ABSX,4}},{0x59,{ABSY,4}},{0x41,{INDX,6}},{0x51,{INDY,5}},

    {0xe6,{ZP,5}},{0xf6,{ZPX,6}},{0xee,{ABS,6}},{0xfe,{ABSX,7}},
    {0xe8,{IMP,2}},
    {0xc8,{IMP,2}},

    {0x4c,{ABS,3}},{0x6c,{IND,5}},
    {0x20,{ABS,6}},

    {0xa9,{IMM,2}},{0xa5,{ZP,3}},{0xb5,{ZPX,4}},{0xad,{ABS,4}},{0xbd,{ABSX,4}},{0xb9,{ABSY,4}},{0xa1,{INDX,6}},{0xb1,{INDY,5}},
    {0xa2,{IMM,2}},{0xa6,{ZP,3}},{0xb6,{ZPY,4}},{0xae,{ABS,4}},{0xbe,{ABSY,4}},
    {0xa0,{IMM,2}},{0xa4,{ZP,3}},{0xb4,{ZPX,4}},{0xac,{ABS,4}},{0xbc,{ABSX,4}},
    {0x4a,{ACC,2}},{0x46,{ZP,5}},{0x56,{ZPX,6}},{0x4e,{ABS,6}},{0x5e,{ABSX,7}},

    {0xea,{IMP,2}},

    {0x09,{IMM,2}},{0x05,{ZP,3}},{0x15,{ZPX,4}},{0x0d,{ABS,4}},{0x1d,{ABSX,4}},{0x19,{ABSY,4}},{0x01,{INDX,6}},{0x11,{INDY,5}},

    {0x48,{IMP,3}},
    {0x08,{IMP,3}},
    {0x68,{IMP,4}},
    {0x28,{IMP,4}},

    {0x2a,{ACC,2}},{0x26,{ZP,5}},{0x36,{ZPX,6}},{0x2e,{ABS,6}},{0x3e,{ABSX,7}},
    {0x6a,{ACC,2}},{0x66,{ZP,5}},{0x76,{ZPX,6}},{0x6e,{ABS,6}},{0x7e,{ABSX,7}},
    {0x40,{IMP,6}},
    {0x60,{IMP,6}},

    {0xe9,{IMM,2}},{0xe5,{ZP,3}},{0xf5,{ZPX,4}},{0xed,{ABS,4}},{0xfd,{ABSX,4}},{0xf9,{ABSY,4}},{0xe1,{INDX,6}},{0xf1,{INDY,5}},
    {0x38,{IMP,2}},
    {0x78,{IMP,2}},
    {0x85,{ZP,3}},{0x95,{ZPX,3}},{0x8d,{ABS,4}},{0x9d,{ABSX,5}},{0x99,{ABSY,5}},{0x81,{INDX,6}},{0x91,{INDY,6}},
    {0x86,{ZPX,3}},{0x96,{ZPY,4}},{0x8e,{ABS,4}},
    {0x84,{ZP,3}},{0x94,{ZPX,4}},{0x8c,{ABS,4}}




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
            case 0xc9:case 0xc5:case 0xd5:case 0xcd:case 0xdd:case 0xd9:case 0xc1:case 0xd1:CMP();break;
            case 0xe0:case 0xe4:case 0xec:CPX();break;
            case 0xc0:case 0xc4:case 0xcc:CPY();break;
            case 0xc6:case 0xd6:case 0xc3:case 0xde:DEC();break;
            case 0xca:DEX();break;
            case 0x88:DEY();break;
            case 0x49:case 0x45:case 0x55:case 0x4d:case 0x5d:case 0x59:case 0x41:case 0x51:EOR();break;
            case 0xe6:case 0xf6:case 0xee:case 0xfe:INC();
            case 0xe8:INX();break;
            case 0xc8:INY();break;
            case 0x4c:case 0x6c:JMP();break;
            case 0x20:JSR();break;
            case 0xa9:case 0xa5:case 0xb5:case 0xad:case 0xbd:case 0xb9:case 0xa1:case 0xb1:LDA();break;
            case 0xa2:case 0xa6:case 0xb6:case 0xae:case 0xbe:LDX();break;
            case 0xa0:case 0xa4:case 0xb4:case 0xac:case 0xbc:LDY();break;
            case 0x4a:case 0x46:case 0x56:case 0x4e:case 0x5e:LSR();break;
            case 0xea:NOP();break;
            case 0x09:case 0x05:case 0x15:case 0x0d:case 0x1d:case 0x19:case 0x01:case 0x11:ORA();break;
            case 0x48:PHA();break;
            case 0x08:PHP();break;
            case 0x68:PLA();break;
            case 0x28:PLP();break;
            case 0x2a:case 0x26:case 0x36:case 0x2e:case 0x3e:ROL();break;
            case 0x6a:case 0x66:case 0x76:case 0x6e:case 0x7e:ROR();break;
            case 0x40:RTI();break;
            case 0x60:RTS();break; 
            case 0xe9:case 0xe5:case 0xf5:case 0fed:case 0xfd:case 0xf9:case 0xe1:case 0xf1:SBC();break;
            case 0x38:SEC();break;
            case 0xf8:SED();break;
            case 0x78:SEI();break;
            case 0x85:case 0x95:case 0x8d:case 0x9d:case 0x99:case 0x81:case 0x91:STA();break;
            case 0x86:case 0x96:case 0x8e:STX();break;
            case 0x84:case 0x94:case 0x8c:STY();break;
            
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

byte cpu::pop_stack()
{
    byte ret = _bus->read((byte_2)0x100 + (byte_2)S);
    S++;
    return ret;
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

void cpu::CPX()
{
    byte oper = find_operator_by_mode(opcode_map[OPCODE].mode);
    byte_2 result = (byte_2)X - (byte_2)oper;
    generate_NCZ_flags(0x83,result);
}

void cpu::CPY()
{
    byte oper = find_operator_by_mode(opcode_map[OPCODE].mode);
    byte_2 result = (byte_2)Y - (byte_2)oper;
    generate_NCZ_flags(0x83,result);
}

void cpu::DEC()
{
    byte_2 adress = find_adress_by_mode(opcode_map[OPCODE].mode);
    byte oper = _bus->read(adress);
    oper--;
    _bus->write(adress , oper);
    generate_NCZ_flags(0x82, oper);
}

void cpu::DEX()
{
    X--;
    generate_NCZ_flags(0x82,X);
}

void cpu::DEY()
{
    Y--;
    generate_NCZ_flags(0x82,Y);
}

void cpu::EOR()
{
    byte oper = find_operator_by_mode(opcode_map[OPCODE].mode);
    byte result = A ^ oper;
    A = result;
    generate_NCZ_flags(0x82,(byte_2)A);
}

void cpu::INC()
{
    byte_2 adress = find_adress_by_mode(opcode_map[OPCODE].mode);
    byte oper = _bus->read(adress);
    oper++;
    _bus->write(adress , oper);
    generate_NCZ_flags(0x82, oper);
}

void cpu::INX()
{
    X++;
    generate_NCZ_flags(0x82,X);
}

void cpu::INY()
{
    Y++;
    generate_NCZ_flags(0x82,Y);
}

void cpu::JMP()
{
    byte_2 adress = find_adress_by_mode(opcode_map[OPCODE].mode);
    PC = adress;
}

void cpu::JSR()
{
    byte_2 ret = PC + 2;
    push_stack((byte)(ret >> 8));
    push_stack((byte)(ret % 256));
    
    PC = find_adress_by_mode(ABS);

}

void cpu::LDA()
{
    A = find_operator_by_mode(opcode_map[OPCODE].mode);
    generate_NCZ_flags(0x82,A);
}

void cpu::LDX()
{
    X = find_operator_by_mode(opcode_map[OPCODE].mode);
    generate_NCZ_flags(0x82,X);
}

void cpu::LDY()
{
    Y = find_operator_by_mode(opcode_map[OPCODE].mode);
    generate_NCZ_flags(0x82,Y);
}

void cpu::LSR()
{
    instruction info = opcode_map[OPCODE];
    
    if(info.mode == ACC)
    {
        A = A >> 1;
        generate_NCZ_flags(0x83 , (byte_2)A);
    }
    else{
        byte_2 adress = find_adress_by_mode(info.mode);
        byte oper = _bus->read(adress);
        oper = oper >> 1;
        _bus->write(adress , oper);
        generate_NCZ_flags(0x83 , (byte_2)oper);
    }
}

void cpu::ORA()
{
    instruction info = opcode_map[OPCODE];
    byte oper = find_operator_by_mode(info.mode);
    A = A | oper;
    generate_NCZ_flags(0x82, A);
}

void cpu::PHA()
{
    push_stack(A);
}

void cpu::PHP()
{
    push_stack(P);
}

void cpu::PLA()
{
    A = pop_stack();
    generate_NCZ_flags(0x82, A);
}

void cpu::PLP()
{
    P = pop_stack();
}

void cpu::ROL()
{
    ADR mode = opcode_map[OPCODE].mode;
    byte_2 adress;
    byte oper;
    if(mode != ACC)
    {
        adress = find_adress_by_mode(mode);
        oper = _bus->read(adress);
    }
    else{
        oper = A;
    }

    bool last_bit = ACTIVE_BIT(oper , 7);
    oper = oper << 1;
    if(last_bit) oper |= 0x01;
    byte_2 result = (byte_2)oper;
    if(last_bit) result |= 0x0100;
    
    if(mode == ACC) A = oper;
    else{
        _bus->write(adress , oper);
    }
    generate_NCZ_flags(0x83 , result);
}

void cpu::ROR()
{
    ADR mode = opcode_map[OPCODE].mode;
    byte_2 adress;
    byte oper;
    if(mode != ACC)
    {
        adress = find_adress_by_mode(mode);
        oper = _bus->read(adress);
    }
    else{
        oper = A;
    }

    bool first_bit = ACTIVE_BIT(oper , 0);
    oper = oper >> 1;
    if(first_bit) oper |= 0x80;
    byte_2 result = (byte_2)oper;
    if(first_bit) result |= 0x0100;
    
    if(mode == ACC) A = oper;
    else{
        _bus->write(adress , oper);
    }
    generate_NCZ_flags(0x83 , result);
}

void cpu::RTI()
{
    P = pop_stack();
    byte pc1 = pop_stack();
    byte pc2 = pop_stack();

    PC = (byte_2)pc1 * (byte_2)256 + (byte_2)pc2;
}

void cpu::RTS()
{
    byte pc1 = pop_stack();
    byte pc2 = pop_stack();

    PC = (byte_2)pc1 * (byte_2)256 + (byte_2)pc2;
}

void cpu::SBC()
{
    byte oper = find_operator_by_mode(opcode_map[OPCODE].mode);
    byte_2 result = (byte_2)A + (byte_2)oper + (byte_2)(P & 0x01);
    A = (byte)(result >> 8);
    generate_NCZ_flags(0x83 , result);
    generate_overflow_flag((byte_2)A + (byte_2)(P & 0x01) , (byte_2)oper);
    
}

void cpu::STA()
{
    byte_2 adress = find_adress_by_mode(opcode_map[OPCODE].mode);
    _bus->write(adress , A);
}

void cpu::STX()
{
    byte_2 adress = find_adress_by_mode(opcode_map[OPCODE].mode);
    _bus->write(adress , X);
}

void cpu::STY()
{
    byte_2 adress = find_adress_by_mode(opcode_map[OPCODE].mode);
    _bus->write(adress , Y);
}