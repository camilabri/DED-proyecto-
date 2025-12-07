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

void agregarLibro();
void agregarUsuario();
boolean prestarLibro ();
boolean devolverLibro();
void mostrarLibros ();
void mostrarUsuarios();
void mostrarTopLibros ();


#endif