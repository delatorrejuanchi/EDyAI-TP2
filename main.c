#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// TIMING
#include <sys/resource.h>
#include <sys/times.h>
double calculate(struct rusage* b, struct rusage* a) {
  if (b == NULL || a == NULL)
    return 0;
  else
    return ((((a->ru_utime.tv_sec * 1000000 + a->ru_utime.tv_usec) -
              (b->ru_utime.tv_sec * 1000000 + b->ru_utime.tv_usec)) +
             ((a->ru_stime.tv_sec * 1000000 + a->ru_stime.tv_usec) -
              (b->ru_stime.tv_sec * 1000000 + b->ru_stime.tv_usec))) /
            1000000.);
}

#define TAMANO_TABLAHASH 1499

typedef struct _SNodo {
  char* dato;
  struct _SNodo* sig;
} SNodo;

typedef SNodo* SList;

SList slist_crear() { return NULL; }

int slist_vacia(SList slist) { return slist == NULL; }

SList slist_agregar_inicio(SList slist, char* dato) {
  SNodo* nuevoNodo = malloc(sizeof(SNodo));
  nuevoNodo->dato = dato;
  nuevoNodo->sig = slist;

  return nuevoNodo;
}

SList slist_eliminar_inicio(SList slist) {
  SNodo* nodoAEliminar = slist;
  slist = slist->sig;

  free(nodoAEliminar->dato);
  free(nodoAEliminar);
  return slist;
}

void slist_destruir(SList slist) {
  while (slist != NULL) {
    slist = slist_eliminar_inicio(slist);
  }
}

void slist_imprimir(SList slist) {
  for (SNodo* nodo = slist; nodo != NULL; nodo = nodo->sig) {
    printf("%s ", nodo->dato);
  }

  printf("\n");
}

int slist_longitud(SList slist) {
  int longitud = 0;
  for (SNodo* nodo = slist; nodo != NULL; nodo = nodo->sig) {
    longitud++;
  }

  return longitud;
}

int slist_buscar_dato(SList slist, char* dato) {
  SNodo* nodo;

  for (nodo = slist; nodo != NULL && strcmp(nodo->dato, dato) != 0;
       nodo = nodo->sig)
    ;

  return nodo != NULL;
}

typedef unsigned int (*FuncionHash)(char* dato);

typedef struct {
  SList* tabla;
  unsigned int numElems;
  unsigned int capacidad;
  FuncionHash hash;
} TablaHash;

TablaHash* tablahash_crear(unsigned int capacidad, FuncionHash hash) {
  TablaHash* tabla = malloc(sizeof(TablaHash));
  tabla->tabla = malloc(sizeof(SList*) * capacidad);
  for (int i = 0; i < capacidad; i++) {
    tabla->tabla[i] = slist_crear();
  }

  tabla->capacidad = capacidad;
  tabla->numElems = 0;
  tabla->hash = hash;

  return tabla;
}

void tablahash_insertar(TablaHash* tabla, char* dato) {
  unsigned id = tabla->hash(dato) % tabla->capacidad;
  tabla->tabla[id] = slist_agregar_inicio(tabla->tabla[id], dato);
  // TODO: numElems incrementa siempre o solo cuando llenamos un casillero
  // nuevo?
  tabla->numElems++;
}

int tablahash_buscar(TablaHash* tabla, char* dato) {
  unsigned id = tabla->hash(dato) % tabla->capacidad;

  return slist_buscar_dato(tabla->tabla[id], dato);
}

void tablahash_destruir(TablaHash* tabla) {
  for (int i = 0; i < tabla->capacidad; i++) {
    slist_destruir(tabla->tabla[i]);
  }
  free(tabla->tabla);
  free(tabla);
}

unsigned int hash(char* dato) {
  int suma = 0;
  for (int i = 0; i < strlen(dato); i++) {
    suma += dato[i];
  }

  return suma;
  // return TAMANO_TABLAHASH - (suma % TAMANO_TABLAHASH);
}

// unsigned int hash(char* dato) { return dato[0] + strlen(dato); }

int cargar_tabla(TablaHash* tabla, char* archivo) {
  FILE* diccionario = fopen(archivo, "r");

  if (diccionario == NULL) {
    return 0;
  }
  // TODO: que tamaño uso para las palabras?
  char buffer[50];
  while (fscanf(diccionario, "%s", buffer) != EOF) {
    if (!tablahash_buscar(tabla, buffer)) {
      char* palabra = malloc(sizeof(char) * (strlen(buffer) + 1));

      // TODO: pasamos a lowercase?
      for (int i = 0; i < strlen(buffer); i++) {
        buffer[i] = tolower(buffer[i]);
      }
      strcpy(palabra, buffer);

      tablahash_insertar(tabla, palabra);
    }
  }

  fclose(diccionario);

  return 1;
}

// int corregir_texto(TablaHash* tabla, char* archivo) {
//   FILE* texto = fopen(archivo, "r");

//   if (texto == NULL) {
//     return 0;
//   }

//   char buffer[50], c;
//   int i = 0;
//   while ((c = fgetc(texto)) != EOF) {
//     if (isalpha(c)) {
//       buffer[i] = c;
//       i++;
//     }

//     if (c == ' ') {

//       char* palabra = malloc
//     }
//   }

//   fclose(texto);

//   return 1;
// }

SList agregar_sugerencia(SList sugerencias, char* sugerencia) {
  char* palabra = malloc(sizeof(char) * (strlen(sugerencia) + 1));
  strcpy(palabra, sugerencia);

  return slist_agregar_inicio(sugerencias, palabra);
}

int main(int argc, char* argv[]) {
  if (argc != 3) {
    printf("Error: el número de argumentos ingresados es incorrecto.\n");
    printf("Modo de uso: %s [diccionario.txt] [texto.txt]", argv[0]);
    return 1;
  }

  struct rusage before, after;

  TablaHash* tabla = tablahash_crear(TAMANO_TABLAHASH, hash);
  // TODO: preguntar como determinar un buen tamaño. Se que tiene que ser primo.
  // TODO: debemos inicializar a NULL cada SList dentro de tabla->tabla?

  getrusage(RUSAGE_SELF, &before);
  int cargada = cargar_tabla(tabla, argv[1]);
  if (!cargada) {
    printf("Error: el archivo %s no existe.\n", argv[1]);
    tablahash_destruir(tabla);
    return 1;
  }
  getrusage(RUSAGE_SELF, &after);
  printf("Tiempo de carga: %f\n", calculate(&before, &after));

  // getrusage(RUSAGE_SELF, &before);
  FILE* out = fopen("out.txt", "w+");
  for (int i = 0; i < TAMANO_TABLAHASH; i++) {
    fprintf(out, "%d %d\n", i, slist_longitud(tabla->tabla[i]));
    // if (slist_longitud(tabla->tabla[i]) >= 200)
    // slist_imprimir(tabla->tabla[i]);
  }
  fclose(out);
  // getrusage(RUSAGE_SELF, &after);
  // printf("Tiempo para recorrer: %f\n", calculate(&before, &after));

  // getrusage(RUSAGE_SELF, &before);
  // int corregido = corregir_texto(tabla, argv[2]);
  // if (!corregido) {
  //   printf("Error: el archivo %s no existe.\n", argv[2]);
  //   tablahash_destruir(tabla);
  //   return 1;
  // }

  char palabra[50];
  while (strcmp(palabra, "exit") != 0) {
    scanf("%s", palabra);

    getrusage(RUSAGE_SELF, &before);
    int correcta = tablahash_buscar(tabla, palabra);
    getrusage(RUSAGE_SELF, &after);
    printf("Tiempo para buscar: %f\n", calculate(&before, &after));
    if (correcta == 0) {
      printf("\"%s\" no está en el diccionario.\n", palabra);
      SList sugerencias = slist_crear();

      int longitud = strlen(palabra);

      // TODO: no checkear la misma palabra longitud veces xd
      // Cambiando una letra
      for (int i = 0; i < longitud; i++) {
        char original = palabra[i];
        for (int c = 'a'; c <= 'z'; c++) {
          palabra[i] = c;
          if (tablahash_buscar(tabla, palabra)) {
            sugerencias = agregar_sugerencia(sugerencias, palabra);
          }
        }
        palabra[i] = original;
      }

      // TODO: no agrega al inicio
      // Agregando una letra
      for (int i = longitud; i > 0; i--) {
        palabra[i + 1] = palabra[i];
      }
      for (int i = 1; i < longitud + 1; i++) {
        for (int c = 'a'; c <= 'z'; c++) {
          palabra[i] = c;
          if (tablahash_buscar(tabla, palabra)) {
            char* sugerencia = malloc(sizeof(char) * (strlen(palabra) + 1));
            strcpy(sugerencia, palabra);

            sugerencias = slist_agregar_inicio(sugerencias, sugerencia);
          }
        }
        palabra[i] = palabra[i + 1];
      }
      palabra[longitud] = '\0';

      // Eliminando un caracter
      char eliminado = palabra[0];
      for (int i = 0; i < longitud; i++) palabra[i] = palabra[i + 1];
      for (int i = 0; i < longitud; i++) {
        if (tablahash_buscar(tabla, palabra)) {
          sugerencias = agregar_sugerencia(sugerencias, palabra);
        }

        char tmp = eliminado;
        eliminado = palabra[i];
        palabra[i] = tmp;
      }
      palabra[strlen(palabra)] = eliminado;
      palabra[strlen(palabra) + 1] = '\0';

      // Separando la palabra
      for (int i = longitud; i > 0; i--) {
        palabra[i + 1] = palabra[i];
      }
      for (int i = 1; i < longitud; i++) {
        palabra[i] = '\0';

        if (tablahash_buscar(tabla, palabra) &&
            tablahash_buscar(tabla, palabra + i + 1)) {
          palabra[i] = ' ';
          sugerencias = agregar_sugerencia(sugerencias, palabra);
        }

        palabra[i] = palabra[i + 1];
      }
      palabra[strlen(palabra) - 1] = '\0';

      if (!slist_vacia(sugerencias)) {
        printf("Quizás quiso decir: ");
        slist_imprimir(sugerencias);
      }
      slist_destruir(sugerencias);
    } else {
      printf("Palabra encontrada!\n");
    }
  }

  // getrusage(RUSAGE_SELF, &after);
  // printf("Tiempo para corregir: %f\n", calculate(&before, &after));

  tablahash_destruir(tabla);

  return 0;
}
