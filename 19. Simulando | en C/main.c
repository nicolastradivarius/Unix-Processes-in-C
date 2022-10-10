#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <sys/wait.h>
#include <time.h>
#include <string.h>

/**
 * Vamos a tener un proceso principal que creará un proceso para "grep", otro para "ping", y creará también un pipe entre ellos.
*/
int main(int argc, char const *argv[]) {
    int fd[2];
    if(pipe(fd) == -1) {
        return 1;
    }

    int pid1 = fork();
    if (pid1 < 0) {
        return 2;
    }

    if (pid1 == 0) {
        // Child process 1 (ping)
        dup2(fd[1], STDOUT_FILENO); //el standard output ahora es el punto de escritura del pipe. ping escribirá los datos de salida en el pipe.
        close(fd[0]);
        close(fd[1]); //lo cerramos porque sigue abierto después de duplicarlo.
        execlp("ping", "ping", "-c", "1", "google.com", NULL);
    }
    //No necesitamos poner un else porque todo lo que está debajo del execlp no existirá en el proceso hijo dado que se reemplazó por el programa que especificamos en execlp. O sea, sólo será ejecutado por el proceso padre.

    int pid2 = fork();
    if (pid2 < 0) {
        return 3;
    }

    if (pid2 == 0) {
        //Child process 2 (grep)
        dup2(fd[0], STDIN_FILENO); //el standard input ahora es el punto de lectura del pipe (grep leerá los datos de entrada del pipe).
        close(fd[0]);
        close(fd[1]);
        execlp("grep", "grep", "rtt", NULL);
    }

    //Cerramos los puntos de lectura y escritura del pipe en el PROCESO PRINCIPAL, ya que sólo los habíamos cerrado en los procesos hijos. No hacer esto tiene como consecuencia que el proceso "grep" espere en el pipe para obtener input. Sólo cuando cerramos el pipe, grep finaliza su ejecución.
    close(fd[0]);
    close(fd[1]);

    waitpid(pid1, NULL, 0);
    waitpid(pid2, NULL, 0);
    return 0;
}
