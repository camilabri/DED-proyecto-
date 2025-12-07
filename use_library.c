#include "library.h"
#include <stdio.h>

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
  printf("comparing %s with %s\n", c1, c2);
  return strcmp(c1, c2) == 0;
}
int main() {
    sistema * s = system_create();
    int opcion;

    do {
        printf("1. Agregar libro\n");
        printf("2. Agregar usuario\n");
        printf("3. Prestar libro\n");
        printf("4. Devolver libro\n");
        printf("5. Mostrar libros\n");
        printf("6. Mostrar usuarios\n");
        printf("7. Top libros\n");
        printf("0. Salir\n");
        scanf("%d", &opcion);

        switch(opcion) {
            case 1: agregarLibro(s);
            break;
            case 2: agregarUsuario(s); 
             break;
            case 3: prestarLibro(s);
             break;
            case 4: devolverLibro(s);
            break;
            case 5: mostrarLibros (s);
            break;
            case 6: mostrarUsuarios(s);
            break;
            case 7: mostrarTopLibros (s);
            break;
            default:
            break;
        }

    } while(opcion != 0);

    return 0;
}