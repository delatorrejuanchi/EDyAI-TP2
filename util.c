#include "util.h"
#include <stdlib.h>

void destruir_generico(void* dato) { free(dato); }
void no_destruir(void* dato) {}
