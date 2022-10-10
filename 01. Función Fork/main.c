#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>

int main(int argc, char const *argv[])
{
    int id = fork();
//    int id2 = fork();
    //el primer proceso crea un hijo, luego ambos procesos crean un hijo. En total: cuatro procesos y por lo tanto cuatro impresiones por pantalla.
    //printf("Hello world from id: %d\n", id);
    if (id!=0) {
        fork();
        //printf("Hello from main process\n");
    } else {
        //printf("Hello from child process\n");
    }

    printf("Hello world\n");
    return 0;

}
