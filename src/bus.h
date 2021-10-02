#ifndef BUS_H
#define BUS_H

#include "ram.h"
#include "definitions.h"
class bus
{

    public:
    bus();
    byte read(byte_2 adress);
    void write(byte_2 adress , byte data);
    private:

    ram _ram;

    byte data;
    byte_2 adress;
};




#endif