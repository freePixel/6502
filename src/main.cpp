#include <iostream>
#include "cpu.h"

int main()
{
    bus* _bus = new bus();
    cpu* CPU = new cpu(_bus);


    delete _bus;
    delete CPU;
}