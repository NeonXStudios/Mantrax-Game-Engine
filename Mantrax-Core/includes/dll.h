#ifdef GARIN_CORE_EXPORT
#define GARIN_API __declspec(dllexport)
#else
#define GARIN_API __declspec(dllimport)
#endif