#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>

int main(int argc, char const *argv[])
{
    int id = fork();

    if (id == 0) {
        sleep(1); //hacer esto en el proceso hijo hará que el proceso padre finalice, y el proceso hijo se quedaría "sin padre". Sin embargo, un nuevo padre es asignado a este hijo, y es "systemd" (usualmente con PID igual a 1).
        //Para evitar esta situación, hacemos que el padre espere al hijo con wait(NULL)
    }

    printf("Current ID: %d, Parent ID: %d\n", getpid(), getppid());

    /*
    if (wait(NULL) == -1){
        printf("Desde %d: no hay hijos por los que esperar\n", getpid());
    } else {
        printf("Desde %d: hay hijos por los que esperar\n", getpid());
    }
    */

    int res = wait(NULL);
    if (res == -1){
        printf("No hay hijos por los que esperar\n");
    } else {
        printf("%d finalizó la ejecucuón\n", res);
    }
    return 0;
}
