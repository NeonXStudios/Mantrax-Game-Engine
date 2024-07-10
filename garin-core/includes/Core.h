#pragma once
#ifdef _WIN32
#ifdef GARINLIBS_EXPORTS
#define GARINLIBS_API __declspec(dllexport)
#else
#define GARINLIBS_API __declspec(dllimport)
#endif
#else
#define GARINLIBS_API
#endif