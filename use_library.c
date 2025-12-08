#include "library.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

//para compilar: gcc use_library.c library.c MAP/map.c QUEUE/queue.c PQ/pq.c -o ejecutable

int bookHash(void *t) 
{
  int *key = (int *)t;
  int hash = abs(*key);
  return hash;
}

boolean bookEquals(void *a, void *b)
{
    int A = *((int*)a);
    int B = *((int*)b);
    return A == B;
}

int userHash(void *t) {
  char *key = (char *)t;
  int i, hash = 0;
  for (i = 0; i < strlen(key); i++) 
  {
    hash += (key[i] - 'a');
  }
  return hash;
}

boolean userEquals(void * t1, void * t2) {
  char *c1 = (char *)t1;
  char *c2 = (char *)t2;
  //printf("comparing %s with %s\n", c1, c2);
  return strcmp(c1, c2) == 0;
}

int main() {
    int opcion;
    sistema* s = system_create(10, 10, bookHash, userHash, bookEquals, userEquals);

    while (1) {
        printf("\n=== MENU ===\n");
        printf("1. Agregar libro\n");
        printf("2. Agregar usuario\n");
        printf("3. Prestar libro\n");
        printf("4. Devolver libro\n");
        printf("5. Mostrar libros\n");
        printf("6. Mostrar usuarios\n");
        printf("7. Buscar libro por ID\n");
        printf("8. Buscar libro por titulo\n");
        printf("9. Buscar libros de un autor\n");
        printf("10. Top libros\n");
        printf("11. Ver lista de espera de un libro\n");
        printf("12. Ver libros prestados a un usuario\n");
        printf("13. SALIR\n");
        printf("Opcion: ");
        scanf("%d", &opcion);

        if (opcion == 13) break;

        int id, idU, idL;
        char titulo[100], autor[100], nombre[50];

        switch(opcion){
        case 1:
            printf("ID libro: "); scanf("%d",&id);
            printf("Titulo: "); scanf(" %[^\n]", titulo);
            printf("Autor: "); scanf(" %[^\n]", autor);
            agregarLibro(s,id,titulo,autor);
            break;

        case 2:
            printf("ID usuario: "); scanf("%d",&id);
            printf("Nombre: "); scanf(" %[^\n]", nombre);
            agregarUsuario(s,id,nombre);
            break;

        case 3:
            printf("Nombre usuario: "); scanf(" %[^\n]", nombre);
            printf("ID libro: "); scanf("%d",&idL);
            prestarLibro(s,nombre,idL);
            break;

        case 4:
            printf("Nombre usuario: "); scanf(" %[^\n]", nombre);
            printf("ID libro: "); scanf("%d",&idL);
            devolverLibro(s,nombre,idL);
            break;

        case 5:
            mostrarLibros(s);
            break;

        case 6:
            mostrarUsuarios(s);
            break;

        case 7:
            printf("ID libro: "); scanf("%d",&id);
            {
                book* b = buscarLibroId(s,id);
                if(b) printf("Encontrado: %s\n", getTituloLibro(b));
                else printf("No existe\n");
            }
            break;

        case 8:
            printf("Titulo: "); scanf(" %[^\n]", titulo);
            {
                book* b = buscarLibroTitulo(s,titulo);
                if(b) printf("Encontrado: ID %d\n", getIdLibro(b));
                else printf("No existe\n");
            }
            break;

        case 9:
            printf("Autor: "); scanf(" %[^\n]", autor);
            {
                int cant;
                book** lista = buscarLibrosAutor(s,autor,&cant);
                if(!lista) { printf("No hay libros\n"); break; }
                for(int i=0;i<cant;i++)
                    printf("%s\n", getTituloLibro(lista[i]));
                free(lista);
            }
            break;

        case 10:
            mostrarTopLibros(s);
            break;

        case 11:
            printf("ID libro: "); scanf("%d",&idL);
            mostrarListaEspera(s, idL);
            break;

        case 12:
            printf("Nombre usuario: "); scanf(" %[^\n]", nombre);
            mostrarLibrosPrestados(s, nombre);
            break;

        default:
            printf("Opcion no valida.\n");
        }
    }

    eliminarSistema(s);
    return 0;
}
