#ifndef LIBRARY_H
#define LIBRARY_H

#include "MAP/map.h"
#include "PQ/pq.h"
#include "QUEUE/queue.h"

typedef struct system_str sistema;
typedef struct book_str book;
typedef struct user_str user;

int bookHash(void *t);
boolean bookEquals(void *a, void *b);
int userHash(void *t);
boolean userEquals(void *a, void *b);

void agregarLibro(); //hecho
void agregarUsuario(); //hecho
boolean prestarLibro ();
boolean devolverLibro();
void mostrarLibros (); //hecho
void mostrarUsuarios(); //hecho
void mostrarTopLibros ();


#endif