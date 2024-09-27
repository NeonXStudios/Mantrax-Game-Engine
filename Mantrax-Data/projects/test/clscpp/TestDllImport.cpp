#include "TestDllImport.h"

void TestDllImport::on_init()
{
    // Inicialización
}

void TestDllImport::on_tick()
{
    std::cout << "Hello from DLL: " << find_object_by_index(2)->ObjectName << std::endl;
}
