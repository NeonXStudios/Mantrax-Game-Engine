#include "GarinGameCore.h"
#include <GarinBehaviours.h>
#include "../clscpp/ExampleClass.h"
#include "../clscpp/ExampleGame.h"
#include "../clscpp/TestDllImport.h"

REGISTER_CLASS
{
    std::cout << "Register components in DLL" << std::endl;
    GCLASSDLL(ExampleGame);   /*REGISTER COMPONENT EXAMPLE*/
    GCLASSDLL(ExampleClass);  /*REGISTER COMPONENT EXAMPLE*/
    GCLASSDLL(TestDllImport); /*REGISTER COMPONENT EXAMPLE*/
}

ASSIGN_APIS
{
    ASSIGNAPIS();
    std::cout << "Assigning Graphics API" << std::endl;
    std::cout << "Object: " << GameCoreManager::current_scene->objects_worlds.size() << std::endl;
}
