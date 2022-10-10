#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include <time.h>

int main(int argc, char const *argv[]) {
    int pid = fork();
    if (pid == -1) { return -1; }

    //Si llamamos a execlp en el proceso padre, éste es reemplazado y no podrá esperar por el proceso hijo (el hijo quedará zombie). Así que no se puede llamar execlp en el proceso padre, sólo al hijo, y especificar que el padre debe esperar a que el hijo finalice.

    if (pid == 0) {
        //proceso hijo
        int status = execlp("ping", "ping", "-c", "3", "google.con", NULL);
        //execlp("mkfifo", "mkfifo", "sum", NULL);
        //cualquier cosa que pongamos después del execlp no importará porque será reemplazado. Eso hace que perdamos el control de la programación de nuestro proceso hijo.
        //Cómo chequeamos si el programa que llamamos tuvo un error o no existe? execlp sólo reemplazará la memoria del proceso sólo si tiene éxito, si hay un error esto no ocurrirá y simplemente seguirá con la ejecución de nuestro programa.
        if (status == -1) { printf("No se pudo encontrar el programa a ejecutar.\n"); }
    } else {
        //proceso padre
        int waitStatus;
        wait(&waitStatus);
        if (WIFEXITED(waitStatus)) { //si el proceso por el cual esperamos terminó...
            int statusCode = WEXITSTATUS(waitStatus); //statusCode es el valor de retorno del programa que ejecutamos.
            if (statusCode == 0){
                printf("Éxito!\n");
            } else {
                printf("Falló con el código %d!\n", statusCode);
            }
        }
        //Cómo hacemos para saber si el programa que ejecutó y reemplazó al proceso hijo tuvo éxito? 
        printf("Algo de procesamiento va aquí!\n");
    }
    return 0;
}

