#ifdef GUILIB_EXPORT
#define GUILIB_API __declspec(dllexport) 
#else
#define GUILIB_API __declspec(dllimport)
#endif