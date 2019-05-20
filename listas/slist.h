#ifndef __SLIST__
#define __SLIST__

typedef struct _SNodo {
  int dato;
  struct _SNodo *sig;
} SNodo;

typedef SNodo *SList;

SList slist_crear();

int slist_vacia(SList slist);

SList slist_agregar_inicio(SList slist, int dato);

SList slist_eliminar_inicio(SList slist);

void slist_destruir(SList slist);

void slist_imprimir(SList slist);

#endif /* __SLIST__ */
