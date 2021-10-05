#include "ram.h"

ram::ram()
{
    memory.fill((byte)0xea);
    memory[0] = 0xa9;
    memory[1] = 0x55;
    memory[2] = 0x69;
    memory[3] = 0x31;
    memory[4] = 0x8d;
    memory[5] = 0x25;
    memory[6] = 0x04;

}

void ram::write(byte_2 adress , byte data)
{
    memory[adress] = data;
}

byte ram::read(byte_2 adress)
{
    return memory[adress];
}
