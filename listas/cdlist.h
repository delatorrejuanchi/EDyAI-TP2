#ifndef __CDLIST__
#define __CDLIST__

typedef struct _CDNodo {
  int dato;
  struct _CDNodo *ant;
  struct _CDNodo *sig;
} CDNodo;

typedef CDNodo *CDList;

CDList cdlist_crear();

int cdlist_vacia(CDList cdlist);

CDList cdlist_agregar_inicio(CDList cdlist, int dato);

CDList cdlist_agregar_final(CDList cdlist, int dato);

CDList cdlist_eliminar_inicio(CDList cdlist);

void cdlist_destruir(CDList cdlist);

void cdlist_imprimir(CDList cdlist);

#endif /*__CDLIST__ */
