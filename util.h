#if !defined(__UTIL_H__)
#define __UTIL_H__

// Una FDestructora es una función que recibe un puntero a un dato y lo
// destruye. Es usada como argumento de varias funciones que implican eliminar
// un dato, como por ejemplo:
// - slist_eliminar_inicio, slist_destruir
// - cdlist_eliminar_inicio, cdlist_destruir
// - spila_pop, spila_destruir
// - cdcola_desencolar, cdcola_destruir
typedef void (*FDestructora)(void*);

// Una FVisitante es una función que recibe un puntero a un dato y hace algo con
// él (por ejemplo, lo muestra en pantalla). Es usada como argumento de varias
// funciones en las que se recorre una estructura de datos, como por ejemplo:
// - slist_recorrer
// - cdlist_recorrer
// - spila_recorrer
// - cdcola_recorrer
typedef void (*FVisitante)(void*);

// TODO: documentar
typedef void (*FVisitanteExtra)(void*, void*);

// no_destruir: void* -> void
// Recibe un puntero a un dato,
// No lo destruye.
// Esta función es de tipo FDestructora. Es utilizada cuando se desea realizar
// una operación de eliminación sin eliminar el dato de la memoria.
void no_destruir(void* dato);

// destruir_generico: void* -> void
// Recibe un puntero a un dato genérico,
// Lo destruye.
// Esta función es de tipo FDestructora. Es utilizada cuando free(dato) basta
// para eliminar correctamente el dato.
void destruir_generico(void* dato);

// Definimos la siguiente estructura Caracter para usar en una SPila de
// caracteres.
typedef struct {
  char caracter;
} Caracter;

// caracter_crear: char -> Caracter*
// Recibe un caracter,
// Crea un Caracter y devuelve un puntero al mismo.
Caracter* caracter_crear(char c);

// Representamos caracteres de la siguiente forma:
// - Consideramos iguales mayúsculas y minúsculas
// - Establecemos la siguiente biyección entre caracteres y enteros:
//   a <-> 0   b <-> 1   c <-> 2   d <-> 3   e <-> 4   f <-> 5   g <-> 6
//   h <-> 7   i <-> 8   j <-> 9   k <-> 10  l <-> 11  m <-> 12  n <-> 13
//   o <-> 14  p <-> 15  q <-> 16  r <-> 17  s <-> 18  t <-> 19  u <-> 20
//   v <-> 21  w <-> 22  x <-> 23  y <-> 24  z <-> 25  á <-> 26  é <-> 27
//   í <-> 28  ó <-> 29  ú <-> 30  ü <-> 31  ñ <-> 32
// - A cualquier otro caracter, se le asigna un -1. De esta manera filtramos
//   caracteres inválidos.
// Resulta entonces que el tamaño de nuestro alfabeto es 33, por lo que
// definimos:

#define TAMANO_ALFABETO 33

// caracter_a_indice: char -> int
// Recibe un caracter,
// Devuelve el entero que se le asigna según la representación de caracteres.
// - Nota: por la manera en que se representan algunos caracteres especiales,
//   tenemos en cuenta que al recibirlo primero recibimos un char == -61 y luego
//   otro, pero son los dos en conjunto quienes forman el caracter especial. Por
//   ejemplo, cuando recibimos una ñ lo primero que entra es un char == -61 y
//   luego un char == -79. En esta función, ignoramos el -61 devolviendo -1 y
//   consideramos solo el siguiente valor, -79, que determina cual de los
//   caracteres especiales fue ingresado, devolviendo su representación 32.
int caracter_a_indice(char caracter);

// indice_a_caracter: int -> char
// Recibe un entero,
// Devuelve el caracter que se le asigna según la representación de caracteres.
// - Nota 1: Si el entero no se encuentra en la representación, devolvemos un
//   espacio (' '). Esto es útil cuando generamos sugerencias que contienen
//   espacios.
// - Nota 2: por la manera que se representan algunos caracteres especiales,
//   devolvemos solo la porción del caracter que lo determina. Es decir, si
//   recibimos indice == 32, sabemos que buscamos una ñ. La representación real
//   de este caracter esta compuesta de dos char: un -61 y un -79. Esta función
//   devuelve solamente este último, y al ser utilizada esto debe ser tenido en
//   cuenta notando que antes debe agregarse un -61 para que el caracter pueda
//   ser representado correctamente.
char indice_a_caracter(int indice);

#endif  // __UTIL_H_
