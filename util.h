#if !defined(__UTIL__)
#define __UTIL__

typedef void (*FDestructora)(void*);
typedef void (*FVisitante)(void*);

void no_destruir(void* dato);
void destruir_generico(void* dato);

//TODO: Documentar
int caracter_a_indice(char caracter);
char indice_a_caracter(int indice);


#endif  // __UTIL__
