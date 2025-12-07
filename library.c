#include "library.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

struct system_str{
    map *mapaLibros;
    map *mapaUsuarios;
};

struct book_str{
    int id;
    char titulo[100];
    char autor[100];
    int disponible;
    queue *listaEspera;
    int demanda;
};

struct user_str{
    int id;
    char nombre[50];
    queue *librosPrestados;
};

sistema * system_create() {
    sistema *s = malloc(sizeof(sistema));
    s->mapaLibros = map_create(50,bookHash,bookEquals);
    s->mapaUsuarios = map_create(50,userHash,userEquals);
    return s;
}

int compareLibro(void *a, void *b) {
    book *l1 = (book*)a;
    book *l2 = (book*)b;
    return l2->demanda - l1->demanda;
}

void agregarLibro(sistema * s){

}
void agregarUsuario(sistema * s){

}
boolean prestarLibro (sistema * s){

}
boolean devolverLibro(sistema * s){

}
void mostrarLibros (sistema * s){

}
void mostrarUsuarios(sistema * s){

}
void mostrarTopLibros (sistema * s){

}
