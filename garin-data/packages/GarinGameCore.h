#pragma once

#ifdef _WIN32
#ifdef GARINGAMECORE_EXPORTS
#define GARINGAMECORE_API __declspec(dllexport)
#else
#define GARINGAMECORE_API __declspec(dllimport)
#endif
#else
#define GARINGAMECORE_API
#endif

#include <GarinComponents.h>

class GARINGAMECORE_API TestDllImport : public Component  {

};
