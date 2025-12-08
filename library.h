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

sistema * system_create(int M_libros, int M_users, hash_func bookHash,
    hash_func userHash, equals_func bookEquals, equals_func userEquals);
char* getTituloLibro(book* libro);
char* getAutorLibro(book* libro);
int getIdLibro(book* libro);
void agregarLibro(sistema * s, int id, char* titulo, char* autor);
void agregarUsuario(sistema * s, int id, char* username);
boolean prestarLibro(sistema * s, char* username, int idLibro);
boolean devolverLibro(sistema * s, char* username, int idLibro);
void mostrarLibros(sistema * s);
void mostrarUsuarios(sistema * s);
void mostrarTopLibros(sistema * s);
book* buscarLibroId(sistema* s, int id);
book* buscarLibroTitulo(sistema* s, char* titulo);
book** buscarLibrosAutor(sistema *s, char* autor, int* cantidad);
void eliminarSistema(sistema* s);

#endif