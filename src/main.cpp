#include <iostream>
#include "cpu.h"
#include <thread>
int main()
{
    bus* _bus = new bus();
    cpu* CPU = new cpu(_bus);

    while(true)
    {
        CPU->rising_edge_clk();
        std::this_thread::sleep_for(std::chrono::seconds(1));
    }

    delete _bus;
    delete CPU;
}