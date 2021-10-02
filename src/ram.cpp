#include "ram.h"



void ram::write(byte_2 adress , byte data)
{
    memory[adress] = data;
}

byte ram::read(byte_2 adress)
{
    return memory[adress];
}
