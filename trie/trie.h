#if !defined(__TRIE_H__)
#define __TRIE_H__

#include "../cdcola/cdcola.h"
#include "../slist/slist.h"
#include "../spila/spila.h"
#include "../util.h"

// Representamos nuestro diccionario mediante un Trie. Un Trie cuenta con un
// origen, representado por un puntero a un TNodo.
// Un TNodo es un nodo de un Trie. Cada TNodo tiene un padre, un arreglo de
// TAMANO_ALFABETO hijos, un identificador, y un flag que indica si una palabra
// finaliza en este nodo.
// La representación funciona de la siguiente manera:
// - Supongamos que nuestro universo de palabras válidas es "perro", "gato",
//   "perros", "pera".
// - Nuestro Trie va a tener un origen vacío, es decir con padre == NULL,
//   identificador == -1, y termina == 0. El arreglo de hijos va a constar
//   mayoritariamente de punteros NULL, pero en los índices correspondientes a
//   'p' y 'g' va a contener punteros a dos TNodo distintos.
// - Este hijo 'p' tendrá padre == origen, identificador == 15 (p <-> 15,
//   referir a util.h), termina == 0 y un único hijo no nulo correspondiente a
//   una 'e'.
// - Similarmente, el hijo 'g' tendrá padre == origen, identificador ==
//   6 (g <-> 6, referir a util.h), termina == 0 y un único hijo no nulo
//   correspondiente a una 'a'.
// - La idea, es representar todas las palabras del universo letra por letra
//   siguiendo una relación de descendencia. Visualmente, indicando nodos en los
//   cuales termina una palabra con '!', tendríamos:
//
//         ┌> 'g' -> 'a' -> 't' -> 'o'!
//  origen ┤
//         │
//         │                    ┌> 'r' -> 'o'! -> 's'!
//         └> 'p' -> 'e' -> 'r' ┤
//                              └> 'a'!

typedef struct _TNodo {
  struct _TNodo* padre;
  struct _TNodo* hijos[TAMANO_ALFABETO];
  int identificador;
  int termina;
  // int maxProfundidad; TODO: implementar
} TNodo;

typedef struct {
  TNodo* origen;
} Trie;

// TODO: renombrar
// TODO: documentar
typedef struct {
  TNodo* nodo;
  SPila anteriores;
  int i;
} Estructura;

// tnodo_crear: -> TNodo*
// Crea un TNodo con padre == NULL, identificador == -1 y termina == 0,
// Devuelve un puntero al mismo.
TNodo* tnodo_crear();

// TODO: preguntar si es mejor usar una pila en vez de recursión.
// tnodo_destruir: TNodo* -> void
// Recibe un puntero a un TNodo,
// Destruye el mismo y a todos sus hijos.
void tnodo_destruir(TNodo* tnodo);

// tnodo_buscar: TNodo* char* int -> int
// Recibe un puntero a un TNodo, una palabra y un índice,
// Devuelve 1 si logra encontrar la palabra comenzando desde el caracter i-ésimo
// a partir del nodo indicado, 0 en caso contrario.
int tnodo_buscar(TNodo* tnodo, char* palabra, int i);

// trie_crear: -> Trie*
// Crea e inicializa un Trie,
// Devuelve un puntero al mismo.
Trie* trie_crear();

// trie_agregar: Trie char* -> void
// Recibe un puntero a un Trie y una palabra,
// Agrega la palabra al Trie.
void trie_agregar(Trie* trie, char* palabra);

// trie_contiene: Trie char* -> int
// Recibe un puntero a un Trie y una palabra,
// Devuelve 1 si el Trie contiene la cadena, 0 en caso contrario.
int trie_contiene(Trie* trie, char* palabra);

// trie_sugerir: Trie* char* int -> Arreglo*
// Recibe un puntero a un Trie, una palabra y la cantidad de sugerencias a
// realizar,
// Busca sugerencias para la palabra mediante combinaciones de las siguientes
// operaciones:
// TODO: tratar de reducir la cantidad de checkeos repetitivos, evaluar paso a
// paso a ver como hacerlo
// - Agregar una letra
// - Eliminar una letra
// - Cambiar una letra
// - Intercambiar caracteres adyacentes
// - Agregar espacios
// Prioriza como sugerencias aquellas que son conseguidas utilizando la menor
// cantidad de alteraciones posibles. TODO: vale la pena usar una PCola?
// Devuelve un puntero a un Arreglo con cantidadSugerencias palabras.
Arreglo* trie_sugerir(Trie* trie, char* palabra, int cantidadSugerencias);

// trie_destruir: Trie* -> void
// Recibe un puntero a un Trie,
// Lo destruye.
void trie_destruir(Trie* trie);

// __apilar_padres: TNodo* SPila* -> void
// Recibe un puntero a un TNodo y un puntero a una SPila de Caracter(es),
// Agrega a la pila el caracter correspondiente al nodo, el correspondiente al
// padre del nodo, el correspondiente al padre del padre del nodo, ..., hasta
// llegar al origen.
void __apilar_padres(TNodo* tnodo, SPila* caracteres);

// __reconstruir_anterior: void* void* -> void
// Recibe un puntero a un dato (TNodo) y un puntero a un dato extra (SPila),
// Apila un caracter ' ' a la SPila, y luego llama __apilar_padres sobre el
// TNodo* recibido.
// Esta función es de tipo FVisitanteExtra.
void __reconstruir_anterior(void* dato, void* extra);

// __reconstruir: SPila TNodo* char* int -> char*
// Recibe una SPila de TNodo* (anteriores), un puntero a un TNodo (nodoActual),
// una palabra y un índice,
// Construye y devuelve una cadena de la siguiente manera:
// - En cada TNodo* de la pila "anteriores" termina una palabra, por lo que
//   reconstruimos dichas palabras y las encadenamos con espacios -> anterior_i
// - Agregamos a esta cadena la cadena formada al recorrer nodoActual hasta el
//   origen -> prefijo
// - Agregamos a lo anterior la subcadena de palabra desde el caracter i-ésimo
// -> sufijo
// Es decir, la palabra resultante es de la forma:
//   [anterior_n] [anterior_n-1] ... [anterior_1] [prefijo][sufijo]
char* __reconstruir(SPila anteriores, TNodo* nodoActual, char* palabra, int i);

// __agregar_letras: char* Estructura* CDCola* Arreglo* -> void
// Recibe una palabra, un puntero a una Estructura, un puntero a una CDCola y un
// puntero a un Arreglo,
// Prueba agregando letras desde la posición (estructura->i)-ésima de la palabra
// en adelante. Si encuentra una palabra válida, la agrega al Arreglo de
// sugerencias.
// Para cada nuevaLetra que trata de agregar, si
// (estructura->nodo)->hijos[nuevaLetra] != NULL, encola a la CDCola una nueva
// estructura con los siguientes datos:
// - nodo == (estructura->nodo)->hijos[nuevaLetra]
// - anteriores == estructura->anteriores
// - i == "indice en donde se agregó la letra"
void __agregar_letras(char* palabra, Estructura* estructura, CDCola* cola,
                      Arreglo* sugerencias);

// __eliminar_letras: char* Estructura* CDCola* Arreglo* -> void
// TODO: documentar
void __eliminar_letras(char* palabra, Estructura* estructura, CDCola* cola,
                       Arreglo* sugerencias);

void __intercambiar_letras(char* palabra, Estructura* estructura, CDCola* cola,
                           Arreglo* sugerencias);

void __transponer_letras(char* palabra, Estructura* estructura, CDCola* cola,
                         Arreglo* sugerencias);

void __separar_palabras(char* palabra, Estructura* estructura, CDCola* cola,
                        Arreglo* sugerencias, TNodo* origen);
#endif  // __TRIE_H__
