#include "GarinGameCore.h"
#include <GarinBehaviours.h>
#include "../wlibsgpp/GameLibsLoader.h"
#include <InputWrapper.h>

EngineWrapper::GLFWWrapper EngineWrapper::wrapper;

REGISTER_CLASS
{
    GCLASSDLL(ExampleClass); /*REGISTER COMPONENT EXAMPLE*/
}

ASSIGN_APIS
{
    ASSIGNAPIS();
}
