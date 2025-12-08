#ifndef LIBRARY_H
#define LIBRARY_H
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

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

sistema *system_create();
void agregarLibro(sistema *s); //hecho
void agregarUsuario(sistema *s); //hecho
boolean prestarLibro (sistema *s);
boolean devolverLibro(sistema *s);
void mostrarLibros (sistema *s); //hecho
void mostrarUsuarios(sistema *s); //hecho
void mostrarTopLibros (sistema *s); //hecho

#endif