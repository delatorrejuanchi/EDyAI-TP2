#include "trie.h"
#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../spila/spila.h"

// TODO: perrogato -> perro gato a ?

Estructura* estructura_crear(TNodo* tnodo, SPila anteriores, int i) {
  Estructura* estructura = malloc(sizeof(Estructura));
  estructura->nodo = tnodo;
  estructura->anteriores = anteriores;
  estructura->i = i;

  return estructura;
}

// TODO: documentar
void destruir_estructura(void* dato) {
  Estructura* estructura = dato;
  if (estructura->anteriores != NULL)
    spila_destruir(estructura->anteriores, no_destruir);

  free(dato);
}

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
    int indice = caracter_a_indice(palabra[j]);
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

int trie_contiene(Trie* trie, char* palabra) {
  return tnodo_buscar(trie->origen, palabra, 0);
}

Arreglo* trie_sugerir(Trie* trie, char* palabra, int cantidadSugerencias) {
  Arreglo* sugerencias = arreglo_crear(cantidadSugerencias);

  // Inicializamos una cola de "estructura" jejexd TODO: renombrar
  CDCola cola = cdcola_crear();
  cola = cdcola_encolar(cola, estructura_crear(trie->origen, spila_crear(), 0));

  while (sugerencias->nElems < cantidadSugerencias && !cdcola_vacia(cola)) {
    Estructura* estructura = cdcola_primero(cola);

    // __intercambiar_letras(palabra, estructura, &cola, sugerencias);
    // __agregar_letras(palabra, estructura, &cola, sugerencias);
    // __eliminar_letras(palabra, estructura, &cola, sugerencias);
    // __transponer_letras(palabra, estructura, &cola, sugerencias);
    __separar_palabras(palabra, estructura, &cola, sugerencias, trie->origen);

    cola = cdcola_desencolar(cola, destruir_generico);
  }

  cdlist_destruir(cola, destruir_estructura);

  return sugerencias;
}

void trie_destruir(Trie* trie) {
  tnodo_destruir(trie->origen);
  free(trie);
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

void __agregar_letras(char* palabra, Estructura* estructura, CDCola* cola,
                      Arreglo* sugerencias) {
  int longitud = strlen(palabra);
  TNodo* nodo = estructura->nodo;
  SPila anteriores = estructura->anteriores;
  int i = estructura->i;

  int seguir = nodo != NULL && sugerencias->nElems < sugerencias->tamano;
  for (int j = 0; j <= longitud - i && seguir; j++) {
    int indice = caracter_a_indice(palabra[j + i]);

    int c = 0;
    while (c < TAMANO_ALFABETO && (indice != -1 || j == longitud - i)) {
      if (c != indice) {
        if (tnodo_buscar(nodo->hijos[c], palabra, i + j)) {
          char* sugerencia =
              __reconstruir(anteriores, nodo->hijos[c], palabra, j + i);

          if (!arreglo_anadir(sugerencias, sugerencia)) free(sugerencia);
        }

        if (nodo->hijos[c] != NULL) {
          estructura = estructura_crear(nodo->hijos[c], anteriores, i + j);
          *cola = cdcola_encolar(*cola, estructura);
        }
      }
      c++;
    }

    if (indice != -1) nodo = nodo->hijos[indice];
    seguir = nodo != NULL && sugerencias->nElems < sugerencias->tamano;
  }
}

void __eliminar_letras(char* palabra, Estructura* estructura, CDCola* cola,
                       Arreglo* sugerencias) {
  int longitud = strlen(palabra);
  TNodo* nodo = estructura->nodo;
  SPila anteriores = estructura->anteriores;
  int i = estructura->i;

  int seguir = nodo != NULL && sugerencias->nElems < sugerencias->tamano;
  for (int j = 0; j < longitud - i && seguir; j++) {
    int indice = caracter_a_indice(palabra[i + j]);
    int indiceSig = caracter_a_indice(palabra[i + j + 1]);

    int intentar = indice != indiceSig || nodo->hijos[indiceSig] == NULL;
    if (indice != -1 && intentar) {
      if (tnodo_buscar(nodo, palabra, i + j + 1)) {
        char* sugerencia = __reconstruir(anteriores, nodo, palabra, j + i + 1);
        if (!arreglo_anadir(sugerencias, sugerencia)) free(sugerencia);
      }

      estructura = estructura_crear(nodo, anteriores, i + j + 1);
      *cola = cdcola_encolar(*cola, estructura);
    }

    if (indice != -1) nodo = nodo->hijos[indice];
    seguir = nodo != NULL && sugerencias->nElems < sugerencias->tamano;
  }
}

void __intercambiar_letras(char* palabra, Estructura* estructura, CDCola* cola,
                           Arreglo* sugerencias) {
  int longitud = strlen(palabra);
  TNodo* nodo = estructura->nodo;
  SPila anteriores = estructura->anteriores;
  int i = estructura->i;

  int seguir = nodo != NULL && sugerencias->nElems < sugerencias->tamano;
  for (int j = 0; j < longitud - i && seguir; j++) {
    int indice = caracter_a_indice(palabra[j + i]);

    int c = 0;
    while (c < TAMANO_ALFABETO && indice != -1) {
      if (c != indice) {
        if (tnodo_buscar(nodo->hijos[c], palabra, i + j + 1)) {
          char* sugerencia =
              __reconstruir(anteriores, nodo->hijos[c], palabra, j + i + 1);

          if (!arreglo_anadir(sugerencias, sugerencia)) free(sugerencia);
        }

        if (nodo->hijos[c] != NULL) {
          estructura = estructura_crear(nodo->hijos[c], anteriores, i + j + 1);
          *cola = cdcola_encolar(*cola, estructura);
        }
      }

      c++;
    }

    if (indice != -1) nodo = nodo->hijos[indice];
    seguir = nodo != NULL && sugerencias->nElems < sugerencias->tamano;
  }
}

void __transponer_letras(char* palabra, Estructura* estructura, CDCola* cola,
                         Arreglo* sugerencias) {
  int longitud = strlen(palabra);
  TNodo* nodo = estructura->nodo;
  SPila anteriores = estructura->anteriores;
  int i = estructura->i;

  int seguir = nodo != NULL && sugerencias->nElems < sugerencias->tamano;
  for (int j = 0; j < longitud - i - 1 && seguir; j++) {
    int pos = i + j + 1;
    if (caracter_a_indice(palabra[pos]) == -1 && pos < longitud) pos++;

    int indice = caracter_a_indice(palabra[i + j]);
    int indiceSig = caracter_a_indice(palabra[pos]);
    if (indice != -1 && indiceSig != -1 && nodo->hijos[indiceSig] != NULL &&
        nodo->hijos[indiceSig]->hijos[indice] != NULL) {
      TNodo* auxiliar = nodo->hijos[indiceSig]->hijos[indice];
      if (tnodo_buscar(auxiliar, palabra, i + j + 2)) {
        char* sugerencia =
            __reconstruir(anteriores, auxiliar, palabra, j + i + 2);
        if (!arreglo_anadir(sugerencias, sugerencia)) free(sugerencia);
      }

      estructura = estructura_crear(auxiliar, anteriores, i + j + 2);
      *cola = cdcola_encolar(*cola, estructura);
    }

    if (indice != -1) nodo = nodo->hijos[indice];
    seguir = nodo != NULL && sugerencias->nElems < sugerencias->tamano;
  }
}

void __separar_palabras(char* palabra, Estructura* estructura, CDCola* cola,
                        Arreglo* sugerencias, TNodo* origen) {
  int longitud = strlen(palabra);
  TNodo* nodo = estructura->nodo;
  SPila anteriores = estructura->anteriores;
  int i = estructura->i;

  int seguir = nodo != NULL && sugerencias->nElems < sugerencias->tamano;
  for (int j = 0; j < longitud - i && seguir; j++) {
    int indice = caracter_a_indice(palabra[i + j]);
    if (indice != -1 && nodo->hijos[indice] != NULL &&
        nodo->hijos[indice]->termina) {
      anteriores = spila_push(anteriores, nodo->hijos[indice]);
      if (tnodo_buscar(origen, palabra, i + j + 1)) {
        char* sugerencia =
            __reconstruir(anteriores, origen, palabra, j + i + 1);
        if (!arreglo_anadir(sugerencias, sugerencia)) free(sugerencia);
      }

      estructura = estructura_crear(origen, anteriores, i + j + 1);
      *cola = cdcola_encolar(*cola, estructura);
    }

    if (indice != -1) nodo = nodo->hijos[indice];
    seguir = nodo != NULL && sugerencias->nElems < sugerencias->tamano;
  }
}
