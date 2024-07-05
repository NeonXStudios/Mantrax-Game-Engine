#include "TestDllImport.h"
#include <iostream>

void TestDllImport::on_init()
{
    std::cout << "Starting from DLL" << std::endl;
}

void TestDllImport::on_tick()
{
    std::cout << "Hello from DLL" << std::endl;
}