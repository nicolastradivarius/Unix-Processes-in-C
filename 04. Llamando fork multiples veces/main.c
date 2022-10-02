#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include <errno.h>

int main(int argc, char const *argv[])
{
    int id1 = fork();
    int id2 = fork();

    if (id1 == 0) {
        //si estamos en el proceso hijo
        if (id2 == 0){
            //si estamos en el proceso hijo de ese proceso hijo
            printf("Somos el proceso y\n");
        } else {
            //aún seguimos en el proceso hijo, pero éste no es el proceso que no tiene hijos (el más bajo en el árbol)
            printf("Somos el proceso x\n");
        }
    } else {
        //estamos en otro proceso hijo
        if (id2 == 0) {
            //estamos en z
            printf("Somos el proceso z\n");
        } else {
            printf("Somos el proceso padre\n");
        }
    }

    //Como el proceso padre tiene varios procesos hijos, un simple wait(NULL) hará que el padre finalice cuando alguno de sus hijos finalice, dejando a los demás hijos huérfanos (o zombies)
    while(wait(NULL) != -1 || errno != ECHILD) {
        printf("Esperando por un hijo para finalizar.\n");
    }
    return 0;
}

