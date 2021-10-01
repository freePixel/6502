#include "bus.h"

bus::bus()
{
    _ram = new ram();
}

void bus::write(byte_2 adress , byte data)
{
    this->data = data;

    if(adress < 0x2000)  //ram adress
    {
        _ram->write(adress , data);
        //RAM IS MIRRORED THREE TIMES
        _ram->write(adress + 0x0800 , data);
        _ram->write(adress + 0x1000 , data);
        _ram->write(adress + 0x1800 , data);
    }
}

byte bus::read(byte_2 adress)
{
    if(adress < 0x2000)
    {
        data = _ram->read(adress);
    }
    return data;
}