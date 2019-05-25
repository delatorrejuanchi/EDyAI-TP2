#if !defined(__UTIL__)
#define __UTIL__

typedef void (*FDestructora)(void*);
typedef void (*FVisitante)(void*);

void no_destruir(void* dato);
void destruir_generico(void* dato);

#endif  // __UTIL__
