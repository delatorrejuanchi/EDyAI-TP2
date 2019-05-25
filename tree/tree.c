#include "tree.h"
#include <assert.h>
#include <string.h>
#include "../cdcola/cdcola.h"
#include "../spila/spila.h"
#include "../util.h"

// TODO:
// - verdadera strlen
// - struct { TNodo* nodo; Stack<TNodo*> ancestros; int i;}
// - cola para encolar esta struct
// - stack para apilar padres

typedef struct {
  char caracter;
} Caracter;

typedef struct {
  TNodo* nodo;
  SPila ancestros;
  int i;
} Estructura;

Estructura* estructura_crear(TNodo* nodo, SPila ancestros, int i) {
  Estructura* estructura = malloc(sizeof(Estructura));
  estructura->nodo = nodo;
  estructura->ancestros = ancestros;
  estructura->i = i;

  return estructura;
}

Caracter* caracter_crear(char c) {
  Caracter* caracter = malloc(sizeof(caracter));
  caracter->caracter = c;

  return caracter;
}

int caracter_a_indice(char caracter) {
  int indice = -1;
  if ('a' <= caracter && caracter <= 'z')
    indice = caracter - 97;
  else {
    switch ((int)caracter) {
      case -95:
        indice = 26;
        break;
      case -87:
        indice = 27;
        break;
      case -83:
        indice = 28;
        break;
      case -77:
        indice = 29;
        break;
      case -70:
        indice = 30;
        break;
      case -68:
        indice = 31;
        break;
      case -79:
        indice = 32;
        break;
    }
  }
  return indice;
}

char indice_a_caracter(int indice) {
  char caracter = ' ';
  if (0 <= indice && indice <= 25)
    caracter = indice + 97;
  else {
    switch (indice) {
      case 26:
        caracter = -95;
        break;
      case 27:
        caracter = -87;
        break;
      case 28:
        caracter = -83;
        break;
      case 29:
        caracter = -77;
        break;
      case 30:
        caracter = -70;
        break;
      case 31:
        caracter = -68;
        break;
      case 32:
        caracter = -79;
        break;
    }
  }

  return caracter;
}

// SList palabra_a_slist(char* palabra) {
//   SList slist = slist_crear();

//   SNodo* nodo = slist;
//   for (int i = 0; i < strlen(palabra); i++) {
//     int indice = caracter_a_indice(palabra);
//     if (indice != -1) {
//       SNodo* nuevoNodo = slist_agregar_inicio(NULL, indice);
//       if (slist_vacia(slist))
//         nodo = nuevoNodo;
//       else {
//         nodo->sig = nuevoNodo;
//         nodo = nuevoNodo;
//       }
//     }
//   }
// }

TNodo* tnodo_crear() {
  TNodo* tnodo = malloc(sizeof(TNodo));
  tnodo->padre = NULL;
  tnodo->termina = 0;
  // tnodo->maxProfundidad = 0;
  for (int i = 0; i < TAMANO_ALFABETO; i++) {
    tnodo->hijos[i] = NULL;
  }

  return tnodo;
}

int tnodo_buscar(TNodo* nodo, char* palabra, int i) {
  for (int j = i; j < strlen(palabra) && nodo != NULL; j++) {
    int indice = caracter_a_indice(palabra[j]);
    if (indice != -1) nodo = nodo->hijos[indice];
  }

  return nodo != NULL && nodo->termina;
}

void tnodo_destruir(TNodo* tnodo) {
  if (tnodo != NULL) {
    for (int i = 0; i < TAMANO_ALFABETO; i++) {
      tnodo_destruir(tnodo->hijos[i]);
    }

    free(tnodo);
  }
}

Tree* tree_crear() {
  Tree* tree = malloc(sizeof(TNodo));
  tree->origen = tnodo_crear();

  return tree;
}

void tree_destruir(Tree* tree) {
  tnodo_destruir(tree->origen);
  free(tree);
}

int tree_contiene(Tree* tree, char* palabra) {
  return tnodo_buscar(tree->origen, palabra, 0);
}

// void tree_agregar(Tree* tree, char* palabra) {
//   TNodo* nodo = tree->origen;
//   SList slist = palabra_a_slist(palabra);

//   for (int i = 0; i < strlen(palabra); i++) {
//     int indice = caracter_a_indice(palabra[i]);
//     if (indice != -1) {
//       if (nodo->hijos[indice] == NULL) {
//         nodo->hijos[indice] = tnodo_crear();
//         nodo->hijos[indice]->padre = nodo;
//         nodo->hijos[indice]->identificador = indice;
//       }

//       nodo = nodo->hijos[indice];
//     }
//   }

//   nodo->termina = 1;
// }

void tree_agregar(Tree* tree, char* palabra) {
  TNodo* nodo = tree->origen;
  for (int i = 0; i < strlen(palabra); i++) {
    int indice = caracter_a_indice(palabra[i]);
    if (indice != -1) {
      if (nodo->hijos[indice] == NULL) {
        nodo->hijos[indice] = tnodo_crear();
        nodo->hijos[indice]->padre = nodo;
        nodo->hijos[indice]->identificador = indice;
      }

      nodo = nodo->hijos[indice];
    }
  }

  nodo->termina = 1;
}

char* generar_sugerencia(SPila ancestros, TNodo* nodo, char* palabra, int i) {
  SPila caracteres = spila_crear();
  int longitudPrefijo = 0, longitudSufijo = strlen(palabra) - i;

  TNodo* nodoActual = nodo;
  while (nodoActual != NULL) {
    int indice = nodoActual->identificador;
    Caracter* caracter = caracter_crear(indice_a_caracter(indice));
    caracteres = spila_push(caracteres, caracter);
    longitudPrefijo++;
    if (indice < 0) {
      Caracter* especial = caracter_crear(-61);
      caracteres = spila_push(caracteres, especial);
      longitudPrefijo++;
    }
    nodoActual = nodoActual->padre;
  }

  SNodo* ancestro = ancestros;
  while (ancestro != NULL) {
    nodoActual = ancestro->dato;
    while (nodoActual != NULL) {
      int indice = nodoActual->identificador;
      Caracter* caracter = caracter_crear(indice_a_caracter(indice));
      caracteres = spila_push(caracteres, caracter);
      longitudPrefijo++;
      if (indice < 0) {
        Caracter* especial = caracter_crear(-61);
        caracteres = spila_push(caracteres, especial);
        longitudPrefijo++;
      }
      nodoActual = nodoActual->padre;
    }

    Caracter* espacio = caracter_crear(' ');
    caracteres = spila_push(caracteres, espacio);
    longitudPrefijo++;
    ancestro = ancestro->sig;
  }

  char* sugerencia =
      malloc(sizeof(char) * (longitudPrefijo + longitudSufijo + 1));

  int c = 0;
  while (!spila_vacia(caracteres)) {
    sugerencia[c] = ((Caracter*)spila_top(caracteres))->caracter;
    caracteres = spila_pop(caracteres, destruir_generico);
    c++;
  }

  for (int j = i; j < strlen(palabra); j++) {
    sugerencia[c] = palabra[j];
    c++;
  }

  sugerencia[c] = '\0';

  return sugerencia;
}

SList tree_sugerir(Tree* tree, char* palabra) {
  int longitud = strlen(palabra);

  // Inicializamos una lista de sugerencias
  SList sugerencias = slist_crear();

  // Inicializamos una cola de "estructura" jejexd TODO: renombrar
  CDCola cola = cdcola_crear();
  cola = cdcola_encolar(cola, estructura_crear(tree->origen, spila_crear(), 0));
  // cola = cdcola_desencolar(cola, destruir_generico);
  // cola = cdcola_desencolar(cola, destruir_generico);

  while (slist_longitud(sugerencias) == 0 && !cdcola_vacia(cola)) {
    Estructura* estructura = cdcola_primero(cola);
    TNodo* nodo = estructura->nodo;
    SPila ancestros = estructura->ancestros;
    int i = estructura->i;
    cola = cdcola_desencolar(cola, destruir_generico);

    // Agregar letra
    TNodo* nodoActual = nodo;
    for (int nivel = 0; nivel <= longitud - i && nodoActual != NULL; nivel++) {
      int indice = caracter_a_indice(palabra[nivel + i]);
      if (indice != -1) {
        for (int c = 0; c < TAMANO_ALFABETO; c++) {
          // if (tnodo_buscar(nodoActual->hijos[c], palabra, nivel)) {
          // agregar a sugerencias
          // char* sugerencia =
          //     generar_sugerencia(ancestros, nodoActual, palabra, nivel);
          // sugerencias = slist_agregar_inicio(sugerencias, sugerencia);
          // }

          estructura =
              estructura_crear(nodoActual->hijos[c], ancestros, nivel + i);
          cola = cdcola_encolar(cola, estructura);
        }

        nodoActual = nodoActual->hijos[indice];
      }
    }

    // Eliminar letra
    // nodoActual = nodo;
    // for (int nivel = 0; nivel < longitud - i; nivel++) {
    //   int indice = caracter_a_indice(palabra[nivel + i + 1]);

    //   if (tnodo_buscar(nodoActual->hijos[indice], palabra, nivel + i + 1)) {
    //     // agregar a sugerencias
    //   }

    //   cola = cdcola_encolar(cola,
    //                         2);  // struct nodo=nodo->hijos[indice],
    //   i = nivel + i +
    //       1

    //       indice = caracter_a_indice(palabra[i + nivel]);
    //   nodoActual = nodoActual->hijos[indice];
    // }

    // // Cambiar letra
    // TNodo* nodoActual = nodo;
    // for (int nivel = 0; nivel < longitud(palabra) - i; nivel++) {
    //   for (int c = 0; c < TAMANO_ALFABETO; c++) {
    //     if (tnodo_buscar(nodoActual->hijos[c], palabra, nivel + i + 1)) {
    //       // agregar a sugerencias
    //     }

    //     cola = cdcola_encolar(cola, 1);
    //     // struct nodo=nodo->hijos[c], i=nivel+i+1
    //   }

    //   int indice = caracter_a_indice(palabra[i + nivel]);
    //   nodoActual = nodoActual->hijos[indice];
    // }

    // // Agrregar espacios
    // TNodo* nodoActual = nodo;
    // for (int nivel = 0; nivel <= longitud(palabra) - i; nivel++) {
    //   if (nodoActual->termina && tnodo_buscar(tree->origen, palabra, i + 1))
    //   {
    //     // agregar a sugerencias
    //   }

    //   cola = cdcola_encolar(cola, 1);  // struct nodo=tree->origen,
    //   i = nivel + i + 1,
    //   // padres=padres+nodoActual

    //       int indice = caracter_a_indice(palabra[i + nivel]);
    //   nodoActual = nodoActual->hijos[indice];
    // }
  }

  return sugerencias;
}
