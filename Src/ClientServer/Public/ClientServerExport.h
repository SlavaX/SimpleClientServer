#ifdef CLIENTSERVER_EXPORTS
#define SOCKETDLL __declspec(dllexport)  
#else
#define SOCKETDLL __declspec(dllimport) 
#endif

