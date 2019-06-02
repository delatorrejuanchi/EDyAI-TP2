#include "trie.h"
#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../spila/spila.h"

TNodo* tnodo_crear() {
  TNodo* tnodo = malloc(sizeof(TNodo));
  tnodo->padre = NULL;
  tnodo->identificador = -1;
  tnodo->termina = 0;
  // tnodo->maxProfundidad = 0;

  for (int i = 0; i < TAMANO_ALFABETO; i++) tnodo->hijos[i] = NULL;

  return tnodo;
}

void tnodo_destruir(TNodo* tnodo) {
  if (tnodo == NULL) return;

  for (int i = 0; i < TAMANO_ALFABETO; i++) tnodo_destruir(tnodo->hijos[i]);
  free(tnodo);
}

int tnodo_buscar(TNodo* tnodo, char* palabra, int i) {
  for (int j = i; j < strlen(palabra) && tnodo != NULL; j++) {
    int indice = caracter_a_indice(palabra[j], 1);
    if (indice != -1) tnodo = tnodo->hijos[indice];
  }

  return tnodo != NULL && tnodo->termina;
}

Trie* trie_crear() {
  Trie* trie = malloc(sizeof(TNodo));
  trie->origen = tnodo_crear();

  return trie;
}

void trie_agregar(Trie* trie, char* palabra) {
  TNodo* nodo = trie->origen;
  for (int i = 0; i < strlen(palabra); i++) {
    int indice = caracter_a_indice(palabra[i], 1);
    if (indice != -1) {
      if (nodo->hijos[indice] == NULL) {
        nodo->hijos[indice] = tnodo_crear();
        nodo->hijos[indice]->padre = nodo;
        nodo->hijos[indice]->identificador = indice;
      }

      nodo = nodo->hijos[indice];
    }
  }

  if (nodo != trie->origen) nodo->termina = 1;
}

int trie_contiene(Trie* trie, char* palabra) {
  return tnodo_buscar(trie->origen, palabra, 0);
}

Arreglo* trie_sugerir(Trie* trie, char* palabra, int cantidadSugerencias) {
  Arreglo* sugerencias = arreglo_crear(cantidadSugerencias);

  int maxProfundidad = 0;
  while (!arreglo_lleno(sugerencias)) {
    maxProfundidad++;
    __transformar(palabra, trie->origen, spila_crear(), 0, trie->origen,
                  sugerencias, maxProfundidad);
  }

  return sugerencias;
}

void trie_destruir(Trie* trie) {
  tnodo_destruir(trie->origen);
  free(trie);
}

void __transformar(char* palabra, TNodo* nodo, SPila anteriores, int i,
                   TNodo* origen, Arreglo* sugerencias, int maxProfundidad) {
  if (nodo == NULL || maxProfundidad == 0) return;

  __agregar_letras(palabra, nodo, anteriores, i, origen, sugerencias,
                   maxProfundidad);
  __eliminar_letras(palabra, nodo, anteriores, i, origen, sugerencias,
                    maxProfundidad);
  __intercambiar_letras(palabra, nodo, anteriores, i, origen, sugerencias,
                        maxProfundidad);
  __transponer_letras(palabra, nodo, anteriores, i, origen, sugerencias,
                      maxProfundidad);
  __separar_palabras(palabra, nodo, anteriores, i, origen, sugerencias,
                     maxProfundidad);
}

void __apilar_padres(TNodo* tnodo, SPila* caracteres) {
  TNodo* nodoActual = tnodo;
  while (nodoActual->padre != NULL) {
    int indice = nodoActual->identificador;
    Caracter* caracter = caracter_crear(indice_a_caracter(indice));
    *caracteres = spila_push(*caracteres, caracter);
    if (indice >= 26) {
      Caracter* especial = caracter_crear(-61);
      *caracteres = spila_push(*caracteres, especial);
    }
    nodoActual = nodoActual->padre;
  }
}

void __reconstruir_anterior(void* dato, void* extra) {
  SPila* caracteres = extra;
  Caracter* espacio = caracter_crear(' ');
  *caracteres = spila_push(*caracteres, espacio);
  __apilar_padres((TNodo*)dato, caracteres);
}

char* __reconstruir(SPila anteriores, TNodo* nodoActual, char* palabra, int i) {
  SPila caracteres = spila_crear();
  __apilar_padres(nodoActual, &caracteres);

  spila_recorrer_extra(anteriores, __reconstruir_anterior, &caracteres);

  int prefijo = slist_longitud(caracteres), sufijo = strlen(palabra) - i;
  char* sugerencia = malloc(sizeof(char) * (prefijo + sufijo + 1));

  for (int c = 0; !spila_vacia(caracteres); c++) {
    sugerencia[c] = ((Caracter*)spila_top(caracteres))->caracter;
    caracteres = spila_pop(caracteres, destruir_generico);
  }

  for (int c = 0; c < sufijo; c++) sugerencia[prefijo + c] = palabra[i + c];

  sugerencia[prefijo + sufijo] = '\0';

  return sugerencia;
}

void __agregar_letras(char* palabra, TNodo* nodo, SPila anteriores, int i,
                      TNodo* origen, Arreglo* sugerencias, int maxProfundidad) {
  int longitud = strlen(palabra);
  int seguir = nodo != NULL && !arreglo_lleno(sugerencias);
  for (int j = 0; j <= longitud - i && seguir; j++) {
    int indice = caracter_a_indice(palabra[j + i], 1);

    int c = 0;
    while (c < TAMANO_ALFABETO && (indice != -1 || j == longitud - i)) {
      if (c != indice) {
        if (tnodo_buscar(nodo->hijos[c], palabra, i + j)) {
          char* sugerencia =
              __reconstruir(anteriores, nodo->hijos[c], palabra, j + i);

          if (!arreglo_anadir(sugerencias, sugerencia)) free(sugerencia);
        }

        __transformar(palabra, nodo->hijos[c], anteriores, i + j, origen,
                      sugerencias, maxProfundidad - 1);
      }
      c++;
    }

    if (indice != -1) nodo = nodo->hijos[indice];
    seguir = nodo != NULL && !arreglo_lleno(sugerencias);
  }
}

void __eliminar_letras(char* palabra, TNodo* nodo, SPila anteriores, int i,
                       TNodo* origen, Arreglo* sugerencias,
                       int maxProfundidad) {
  int longitud = strlen(palabra);
  int seguir = nodo != NULL && !arreglo_lleno(sugerencias);
  for (int j = 0; j < longitud - i && seguir; j++) {
    int indice = caracter_a_indice(palabra[i + j], 1);
    int indiceSig = caracter_a_indice(palabra[i + j + 1], 1);

    int intentar = indice != indiceSig || nodo->hijos[indiceSig] == NULL;
    if (indice != -1 && intentar) {
      if (tnodo_buscar(nodo, palabra, i + j + 1)) {
        char* sugerencia = __reconstruir(anteriores, nodo, palabra, j + i + 1);
        if (!arreglo_anadir(sugerencias, sugerencia)) free(sugerencia);
      }

      __transformar(palabra, nodo, anteriores, i + j + 1, origen, sugerencias,
                    maxProfundidad - 1);
    }

    if (indice != -1) nodo = nodo->hijos[indice];
    seguir = nodo != NULL && !arreglo_lleno(sugerencias);
  }
}

void __intercambiar_letras(char* palabra, TNodo* nodo, SPila anteriores, int i,
                           TNodo* origen, Arreglo* sugerencias,
                           int maxProfundidad) {
  int longitud = strlen(palabra);

  int seguir = nodo != NULL && !arreglo_lleno(sugerencias);
  for (int j = 0; j < longitud - i && seguir; j++) {
    int indice = caracter_a_indice(palabra[j + i], 1);

    int c = 0;
    while (c < TAMANO_ALFABETO && indice != -1) {
      if (c != indice) {
        if (tnodo_buscar(nodo->hijos[c], palabra, i + j + 1)) {
          char* sugerencia =
              __reconstruir(anteriores, nodo->hijos[c], palabra, j + i + 1);

          if (!arreglo_anadir(sugerencias, sugerencia)) free(sugerencia);
        }

        __transformar(palabra, nodo->hijos[c], anteriores, i + j + 1, origen,
                      sugerencias, maxProfundidad - 1);
      }

      c++;
    }

    if (indice != -1) nodo = nodo->hijos[indice];
    seguir = nodo != NULL && !arreglo_lleno(sugerencias);
  }
}

void __transponer_letras(char* palabra, TNodo* nodo, SPila anteriores, int i,
                         TNodo* origen, Arreglo* sugerencias,
                         int maxProfundidad) {
  int longitud = strlen(palabra);
  for (int j = 0; j < longitud - i - 1 && !arreglo_lleno(sugerencias); j++) {
    transponer_adyacentes(palabra, i + j);

    if (tnodo_buscar(nodo, palabra, i)) {
      char* sugerencia = __reconstruir(anteriores, nodo, palabra, i);
      if (!arreglo_anadir(sugerencias, sugerencia)) free(sugerencia);
    }
    __transformar(palabra, nodo, anteriores, i, origen, sugerencias,
                  maxProfundidad - 1);

    transponer_adyacentes(palabra, i + j);

    if (caracter_a_indice(palabra[i + j], 1) == -1) j++;
  }
}

void __separar_palabras(char* palabra, TNodo* nodo, SPila anteriores, int i,
                        TNodo* origen, Arreglo* sugerencias,
                        int maxProfundidad) {
  int longitud = strlen(palabra);

  int seguir = nodo != NULL && !arreglo_lleno(sugerencias);
  for (int j = 0; j < longitud - i && seguir; j++) {
    int indice = caracter_a_indice(palabra[i + j], 1);
    if (indice != -1 && nodo->hijos[indice] != NULL &&
        nodo->hijos[indice]->termina) {
      SPila anterioresNuevo = spila_push(anteriores, nodo->hijos[indice]);
      if (tnodo_buscar(origen, palabra, i + j + 1)) {
        char* sugerencia =
            __reconstruir(anterioresNuevo, origen, palabra, j + i + 1);
        if (!arreglo_anadir(sugerencias, sugerencia)) free(sugerencia);
      }

      __transformar(palabra, origen, anterioresNuevo, i + j + 1, origen,
                    sugerencias, maxProfundidad - 1);
      free(anterioresNuevo);
    }

    if (indice != -1) nodo = nodo->hijos[indice];
    seguir = nodo != NULL && !arreglo_lleno(sugerencias);
  }
}
