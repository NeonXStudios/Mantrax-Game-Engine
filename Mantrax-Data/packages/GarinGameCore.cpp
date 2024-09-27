#include "GarinGameCore.h"
#include "TestDllImport.h"
#include <GarinBehaviours.h>

REGISTER_CLASS
{
    // std::cout << "Register components in DLL" << std::endl;
    std::cout << "Register components in DLL" << std::endl;
    GCLASSDLL(TestDllImport);
}

int main()
{
}
