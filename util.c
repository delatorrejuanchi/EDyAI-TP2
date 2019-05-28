#include "util.h"
#include <stdlib.h>

void destruir_generico(void* dato) { free(dato); }
void no_destruir(void* dato) {}

int caracter_a_indice(char caracter) {
  int indice = -1;
  if ('a' <= caracter && caracter <= 'z')
    indice = caracter - 'a';
  else if ('A' <= caracter && caracter <= 'Z')
    indice = caracter - 'A';
  else {
    if (caracter <= -100) caracter -= 32;
    switch ((int)caracter) {
      case -95:
        indice = 26;  // á
        break;
      case -87:
        indice = 27;  // é
        break;
      case -83:
        indice = 28;  // í
        break;
      case -77:
        indice = 29;  // ó
        break;
      case -70:
        indice = 30;  // ú
        break;
      case -68:
        indice = 31;  // ü
        break;
      case -79:
        indice = 32;  // ñ
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
