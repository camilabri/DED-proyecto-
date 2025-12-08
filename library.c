#include "library.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

struct node_str { 
    void * key, * value;
    struct node_str * next;
  };
  
typedef struct node_str node;
  
  struct map_str {//estructura del mapa
    hash_func hash;
    equals_func key_equals;
    int M, size;
    node ** hashTable;
  };



struct system_str{ //estructura del sistema
    map *mapaLibros;
    map *mapaUsuarios;
};

struct book_str{ //estructura del libro
    int id;
    char titulo[100];
    char autor[100];
    int disponible;
    queue *listaEspera; //cola de usuarios esperando este libro
    int demanda; //numero de veces que se ha pedido el libro
};

struct user_str{ //estructura de usuario
    int id;
    char nombre[50];
    map *librosPrestados; //key: id del libro, value: puntero al libro
};

sistema * system_create(int M_libros, int M_users, hash_func bookHash,
                        hash_func userHash, equals_func bookEquals, equals_func userEquals) {
    sistema *s = malloc(sizeof(sistema));
    s->mapaLibros = map_create(M_libros,bookHash,bookEquals);
    s->mapaUsuarios = map_create(M_users,userHash,userEquals);
    return s;
}

int compareLibro(void *a, void *b) {
    book *l1 = (book*)a;
    book *l2 = (book*)b;
    return l2->demanda - l1->demanda;
}

char* getTituloLibro(book* libro){
    return libro->titulo;
}

char* getAutorLibro(book* libro){
    return libro->autor;
}

int getIdLibro(book* libro){
    return libro->id;
}

void agregarLibro(sistema * s, int id, char* titulo, char* autor){
    book *new = malloc(sizeof(book));
    new->id = id;
    strcpy(new->titulo, titulo);
    strcpy(new->autor, autor);
    
    new->disponible = 1; //Disponible para presyat
    new->demanda = 0; //nadie lo ha pedido aún
    new->listaEspera = queue_create(); //cola vacia de usuarios esperandolo

    int *key = malloc(sizeof(int)); //clave dinámica para el mapa (id del libro)
    *key = new->id;

    //insertar al mapa el libro
    map_put(s->mapaLibros, key, new); //insertar libro al mapa
}

void agregarUsuario(sistema * s, int id, char* username){
    user *new = malloc(sizeof(user));
    new->id = id;
    strcpy(new->nombre, username);

    //usamos mismas funcoiones hash y equals para mapa de libro
    new->librosPrestados = map_create(10,s->mapaLibros->hash,s->mapaLibros->key_equals);
 
    char* key = malloc(strlen(username)+1);
    strcpy(key,username);

    map_put(s->mapaUsuarios,key, new);
}

boolean prestarLibro (sistema * s,char* username, int idLibro){
    //verificr que existan usuario y libro
    if(!map_contains(s->mapaUsuarios, username) || !map_contains(s->mapaLibros, &idLibro))
        return FALSE;

    user *u = map_get(s->mapaUsuarios, username);
    book *b = map_get(s->mapaLibros, &idLibro);

    b->demanda++; //incrementamos demanda libro

    if(b->disponible){
        b->disponible = 0; //ya no estará disp si lo prestamos
        
        int * key= malloc(sizeof(int));
        *key = b->id;
        map_put(u->librosPrestados, key,b); //agregamos al mapa
        return TRUE;
    }

    //si no está disponible, agregar usuario a lista de espera
    queue_enqueue(b-> listaEspera, u);
    return FALSE;
}

boolean devolverLibro(sistema * s, char * username, int idLibro){
    //verificar que existan usuario y libro
    if(!map_contains(s->mapaUsuarios, username) || !map_contains(s->mapaLibros, &idLibro))
        return FALSE;

    user* u = map_get(s->mapaUsuarios, username);
    book* b = map_get(s->mapaLibros, &idLibro);

    //verificar que usuario sí tenga el libro prestado
    if(!map_contains(u->librosPrestados, &idLibro))
        return FALSE;
        
    //si hay gente esperando por el libro, prestarlo al siguiente
    if(!queue_isEmpty(b->listaEspera)){
        user* next = queue_dequeue(b->listaEspera);  //siguiente usuario
        
        map_remove(u->librosPrestados, &idLibro); //remover el libro del usuario actual
        
        //prestar el libro al siguiente usuario
        int *key = malloc(sizeof(int));
        *key = b->id;
        map_put(next->librosPrestados, key, b);
        
        b->disponible = 0;  //sigue prestado lol
        return TRUE;
    }
    
    map_remove(u->librosPrestados, &idLibro); //remover del mapa de libros prestados
    b->disponible = 1;  //si no hay nadie esperando, b esta disponible
    
    return TRUE;
}

void mostrarContenido(map *mapa, int tipo) {
    //recorrer buckets de la tabla hash
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
                printf("Cantidad de libros prestados: %d\n", map_size(u->librosPrestados));
                printf("-----------------------------\n");
            }
            n = n->next;
        }
    }
}

void mostrarLibros (sistema * s){
    if(s->mapaLibros->size==0){
        printf("No hay libros que mostrar\n");
        return;
    }
    printf("\n--- LIBROS ---\n");
    mostrarContenido(s->mapaLibros, 1);
}

void mostrarUsuarios(sistema * s){
    if(s->mapaUsuarios->size==0){
        printf("No hay usuarios que mostrar\n");
        return;
    }
    printf("\n--- USUARIOS ---\n");
    mostrarContenido(s->mapaUsuarios, 2);
}


void mostrarTopLibros (sistema * s){
    map* m = s->mapaLibros;
    pq* topLibros = pq_create(m->size,compareLibro); //creamos pq para ordenar libros por demanda (como popularidad)

    int buckets = m->M;
    for(int i=0; i<buckets; i++){
        node* n = m->hashTable[i]; //obtenemos lista ligada de nodos en ese bucket
        while(n){ //recorremos lista x lista hasta que no haya más nodos
            book* libro = (book*)n->value; //casteamos a libro
            pq_offer(topLibros,libro); //agregamos a pq
            n = n->next;
        }
    }

    //para hacer top 5 o menos si tiene menos libros
    int cantidad = pq_size(topLibros)>=5? 5 : pq_size(topLibros); //si son más o igual a 5 hacemos top 5, si son menos hacemos los que hayan

    if(!cantidad){ //si no hay libros
        printf("No hay libros que mostrar\n");
        pq_free(topLibros);
        return;
    }
    
    printf("\n--- TOP %d LIBROS ---\n", cantidad); 
    for(int i=0; i<cantidad; i++){
        book* libro = (book*)pq_poll(topLibros); //extraemos los libros uno por uno 
        printf("  %d - \"%s\", por %s. (Demanda: %d)\n",i+1, libro->titulo, libro->autor, libro->demanda);
    } 

    pq_free(topLibros);
}  

book* buscarLibroId(sistema* s, int id) {
    if (!s) return NULL;
    //buscamos directamente en el mapa con el id 
    book* b = (book*)map_get(s->mapaLibros, &id);
    return b;
}

book* buscarLibroTitulo(sistema* s, char* titulo){
    if(!s || !titulo) return NULL;

    map* m = s->mapaLibros;
    int buckets = m->M;
    //recorremos todo el mapa :( buscando el titulo 
    for(int i = 0; i<buckets; i++){
        node* n = m->hashTable[i];
        while(n){
            book* b = (book*)n->value;
            if(strcmp(b->titulo, titulo) == 0) return b;
            n = n->next;     
        }
    }
    return NULL;
}

book** buscarLibrosAutor(sistema *s, char* autor, int* cantidad){
    map * m = s->mapaLibros;
    *cantidad=0;
    
    //primero vemos si el autor tiene libros 
    for(int i=0; i<m->M; i++){
        node * n= m->hashTable[i];
        while(n){
            book* l = (book*) n->value;
            if(strcmp(l->autor,autor)==0){
                (*cantidad)++;
            }
            n= n->next;
        }
    }
    if(*cantidad==0) return NULL;
    //si sí pues creamos arreglo para almacenar los libros
    book** lista = malloc(sizeof(book*) * (*cantidad));
    int indice=0;
    //segunda pasada y llenamos el arreglo con los libros.. no muy eficiente por ser busqueda lineal 
    for(int i=0; i<m->M; i++){
        node * n= m->hashTable[i];
        while(n){
            book* l = (book*) n->value;
            if(strcmp(l->autor,autor)==0){
                lista[indice++] = l;
            }
            n= n->next;
        }
    }
    return lista;
}

void eliminarSistema(sistema* s){
    if (!s) return;

    //liberar libros
    map* mlib = s->mapaLibros;
    for(int i=0; i<mlib->M; i++){
        node* n = mlib->hashTable[i];

        while(n){
            book* b = (book*)n->value;
            free(n->key); //liberar clave
            if (b->listaEspera) //liberar cola de espera
                queue_destroy(b->listaEspera);

            free(b); //liberar str libro
            n = n->next;
        }
    }
    map_free(mlib); //liberar todo el mapa

    //liberar usuarios
    map* mus = s->mapaUsuarios;
    for(int i=0; i<mus->M; i++){
        node* n = mus->hashTable[i];

        while(n){
            user* u = (user*)n->value;
            free(n->key);

            if(u->librosPrestados){
                for(int j=0;j< u->librosPrestados->M;j++){
                    node* nLibro = u->librosPrestados->hashTable[j];
                    while(nLibro){
                        free(nLibro->key);
                        nLibro = nLibro->next;
                    }
                }
                map_free(u->librosPrestados);
            }
            free(u); //liberar str usuario
            n = n->next;
        }
    }
    map_free(mus);

    //liberar el sistema
    free(s);
}

    map* m = s->mapaLibros;
    int buckets = m->M;
    //recorremos todo el mapa :( buscando el titulo 
    for(int i = 0; i<buckets; i++){
        node* n = m->hashTable[i];
        while(n){
            book* b = (book*)n->value;
            if(strcmp(b->titulo, titulo) == 0) return b;
            n = n->next;     
        }
    }
    return NULL;
}

book** buscarLibrosAutor(sistema *s, char* autor, int* cantidad){
    map * m = s->mapaLibros;
    *cantidad=0;
    
    //primero vemos si el autor tiene libros 
    for(int i=0; i<m->M; i++){
        node * n= m->hashTable[i];
        while(n){
            book* l = (book*) n->value;
            if(strcmp(l->autor,autor)==0){
                (*cantidad)++;
            }
            n= n->next;
        }
    }
    if(*cantidad==0) return NULL;
    //si sí pues creamos arreglo para almacenar los libros
    book** lista = malloc(sizeof(book*) * (*cantidad));
    int indice=0;
    //segunda pasada y llenamos el arreglo con los libros.. no muy eficiente por ser busqueda lineal 
    for(int i=0; i<m->M; i++){
        node * n= m->hashTable[i];
        while(n){
            book* l = (book*) n->value;
            if(strcmp(l->autor,autor)==0){
                lista[indice++] = l;
            }
            n= n->next;
        }
    }
    return lista;
}

void eliminarSistema(sistema* s){
    if (!s) return;

    //liberar libros
    map* mlib = s->mapaLibros;
    for(int i=0; i<mlib->M; i++){
        node* n = mlib->hashTable[i];

        while(n){
            book* b = (book*)n->value;
            free(n->key); //liberar clave
            if (b->listaEspera) //liberar cola de espera
                queue_destroy(b->listaEspera);

            free(b); //liberar str libro
            n = n->next;
        }
    }
    map_free(mlib); //liberar todo el mapa

    //liberar usuarios
    map* mus = s->mapaUsuarios;
    for(int i=0; i<mus->M; i++){
        node* n = mus->hashTable[i];

        while(n){
            user* u = (user*)n->value;
            free(n->key);

            if(u->librosPrestados){
                for(int j=0;j< u->librosPrestados->M;j++){
                    node* nLibro = u->librosPrestados->hashTable[j];
                    while(nLibro){
                        free(nLibro->key);
                        nLibro = nLibro->next;
                    }
                }
                map_free(u->librosPrestados);
            }
            free(u); //liberar str usuario
            n = n->next;
        }
    }
    map_free(mus);

    //liberar el sistema
    free(s);
}
