#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "library.h"

//Revelar la estructura del mapa para poder imprimirlos (mostrarUsuarios y mostrarLibros)   
struct node_str {
    void * key, * value;
    struct node_str * next;
  };
  
typedef struct node_str node;
  
  struct map_str {
    hash_func hash;
    equals_func key_equals;
    int M, size;
    node ** hashTable;
  };

//

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
  book *new = malloc(sizeof(book));
    
    printf("ID del libro: ");
    scanf("%d", &new->id);

    printf("Titulo: ");
    scanf(" %s", new->titulo);

    printf("Autor: ");
    scanf(" %s", new->autor);

    new->disponible = 1;//Disponible para presyat
    new->demanda = 0;//nadie lo ha pedido aún
    new->listaEspera = queue_create();
    int *key = malloc(sizeof(int));
    *key = new->id;

    //insertar al mapa el libro
    map_put(s->mapaLibros, key, new);

    printf("Libro agregado correctamente\n");
}

void agregarUsuario(sistema * s){
    user *new = malloc(sizeof(user));
    
    printf("ID del usuario: ");
    scanf("%d", &new->id);

    printf("Nombre: ");
    scanf(" %s", new->nombre);

    new->librosPrestados = queue_create();
    char *key = malloc(strlen(new->nombre) + 1);
    strcpy(key, new->nombre);
    map_put(s->mapaUsuarios,key, new);

    printf("Usuario agregado correctamente\n");
}
boolean prestarLibro (sistema * s){
//bbuscar libro
//si está disponible: marcar como prestado
//Incrementar demanda
//Añadir al usuario
//Si no está disponible: meter a la cola de espera
}

boolean devolverLibro(sistema * s){
//marcar como disponible
//Si hay gente esperando, prestar al siguiente de la cola
    int id;
    char nombreUsuario[50];

    printf("ID del libro a devolver: ");
    scanf("%d", &id);

    printf("Usuario que devuelve: ");
    scanf(" %s", nombreUsuario);

    //Buscamos libro
    book *libro = (book*)map_get(s->mapaLibros, &id);
    if (!libro){
        printf("Libro con ID %d no existe\n",id);
        return FALSE;
    }

    //buscamos el usuario
    user *usuario = (user*)map_get(s->mapaUsuarios, nombreUsuario);
    if (!usuario){
        printf("Usuario %s no existe\n",nombreUsuario);
        return FALSE;
    }

    //por continuar

}

void mostrarContenido(map *mapa, int tipo) {
    for (int i = 0; i < mapa->M; i++) {
        node *n = mapa->hashTable[i];

        while (n != NULL) { //Lista ligada
            
            if (tipo == 1) { //LIBROS
                book *b= (book*) n->value;
                printf("ID: %d\n", b->id);
                printf("Titulo: %s\n", b->titulo);
                printf("Autor: %s\n", b->autor);
                printf("Disponible: %s\n", b->disponible ? "Disponible":"Rentado");
                printf("Demanda: %d\n", b->demanda);
                printf("----------------------------\n");
            }

            else if (tipo == 2) { //USUARIOS
                user *u = (user*) n->value;
                printf("ID: %d\n", u->id);
                printf("Nombre: %s\n", u->nombre);
                printf("Cantidad de libros prestados: %d\n", queue_size(u->librosPrestados));
                printf("-----------------------------\n");
            }
            n = n->next;
        }
    }
}

void mostrarLibros (sistema * s){
    printf("\n--- LIBROS ---\n");
    mostrarContenido(s->mapaLibros, 1);
}

void mostrarUsuarios(sistema * s){
    printf("\n--- USUARIOS ---\n");
    mostrarContenido(s->mapaUsuarios, 2);
}


void mostrarTopLibros (sistema * s){
    int buckets = s->mapaLibros->M;
    pq* topLibros = pq_create(500,compareLibro);
    for(int i=0; i<buckets; i++){
        node* n = s->mapaLibros->hashTable[i]; //obtenemos lista ligada de nodos en ese bucket
        while(n){ //recorremos lista x lista hasta que no haya más nodos
            book* libro = (book*)n->value; //casteamos a libro
            pq_offer(topLibros,libro);
            n = n->next;
        }
    }

    //para hacer top 5
    int cantidad = pq_size(topLibros)>=5? 5 : pq_size(topLibros); //si son más o igual a 5 hacemos top 5, si son menos hacemos los que hayan

    if(!cantidad){ //si no hay libros
        printf("No hay libros que mostrar\n");
        pq_free(topLibros);
        return;
    }
    
    printf("\n--- TOP %d LIBROS ---\n", cantidad); 
    for(int i=0; i<cantidad; i++){
        book* libro = (book*)pq_poll(topLibros);
        printf("  %d - \"%s\", por %s. (Demanda: %d)\n",i+1, libro->titulo, libro->autor, libro->demanda);
    } 

    pq_free(topLibros);
}


    


