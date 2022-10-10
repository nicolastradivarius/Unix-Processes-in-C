#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <sys/wait.h>
#include <time.h>
#include <string.h>

/**
 * Queremos tener 3 procesos: uno principal, dos secundarios (1 y 2).
 * El proceso principal genera un número y se lo envía al proceso secundario 1, el cual le suma 5.
 * Éste proceso secundario envía el número nuevo al proceso secundario 2, el cual lo multiplicará por 3.
 * Luego el proceso secundario 2 enviará el número nuevo al proceso principal.
 * Necesitamos 3 pipes (porque los pipes son unidireccionales, representan un enlace entre DOS procesos). Los abriremos todos en el proceso principal. Luego tenemos 6 file descriptors (2 por cada pipe, fd[2]).
 * Tener en cuenta que cada proceso forkeado tendrá una copia de esos file descriptors. Luego cada proceso tendrá 6 file descriptors también. Eso significa que habrá 18 file descriptors en total. Con lo cual habrá que llamar a la función close() 18 veces!!
*/
int main(int argc, char const *argv[]) {
    //parent process
    int fd[3][2]; //en lugar de crear 3 variables diferentes para los 3 pipes, por ejemplo fd1 fd2 y fd3, creamos un arreglo donde cada componente es un pipe, y tiene a su vez un arreglo de dos dimensiones donde está el punto de lectura y escritura (por eso el arreglo bidimensional).
    int i;
    for (i = 0; i < 3; i++) {
        if (pipe(fd[i]) < 0) {
            return 1;
        }
    }

    int pid1 = fork();
    if (pid1 < 0) {
        return 2;
    }

    if (pid1 == 0) {
        // Child process 1 --> el que suma 5
        // proceso main ----- proceso secundario 1
        close(fd[0][1]); //no escribimos en el pipe 0 del proceso secundario 1.
        close(fd[1][0]); //no leemos en el pipe 1 del proceso main.
        close(fd[2][0]); 
        close(fd[2][1]); //no leemos ni escribimos en el pipe 2 del proceso secundario 2, no está involucrado aquí.

        int x;
        //leemos del pipe 0 en el proceso secundario 1 (donde el proceso principal escribió el número en fd[0][1])
        if (read(fd[0][0], &x, sizeof(int)) < 0) {
            return 3;
        }
        x += 5;
        printf("Proceso 1: le sumé 5. Resultado parcial: %d\n", x);
        //escribimos en el pipe 1 para que el proceso secundario 2 lo pueda leer despues
        if (write(fd[1][1], &x, sizeof(int)) < 0) {
            return 4;
        }
        close(fd[0][0]);
        close(fd[1][1]);

        //no queremos que el proceso hijo haga más nada después de esto, así que retornamos 0 y el proceso termina
        return 0;
    }

    //no necesitamos poner un else porque el proceso hijo no va a continuar ejecutando después de salir del IF, dado que hay un return. Llega ahí y finaliza su ejecución.

    //parent process

    int pid2 = fork();
    if (pid2 < 0) {
        return 5;
    }

    if (pid2 == 0) {
        // Child process 2: el que lo multiplica por 3
        // proceso secundario 1 ------- proceso secundario 2
        close(fd[0][0]);
        close(fd[0][1]);
        close(fd[1][1]);
        close(fd[2][0]);

        int x;
        //leemos del pipe 1 en el proceso secundario 2 (donde el proceso 1 escribió el número en fd[1][1])
        if (read(fd[1][0], &x, sizeof(int)) < 0) {
            return 6;
        }
        x *= 3;
        printf("Proceso 2: lo multipliqué por 3. Resultado parcial: %d\n", x);
        //escribimos en el pipe 2 para que el proceso principal lo pueda leer despues.
        if (write(fd[2][1], &x, sizeof(int)) < 0) {
            return 7;
        }
        //cerramos los file descriptors usados
        close(fd[1][0]);
        close(fd[2][1]);

        //no queremos que el proceso hijo haga más nada después de esto, así que retornamos 0 y el proceso termina
        return 0;
    }

    //parent process
    close(fd[0][0]);
    close(fd[1][0]);
    close(fd[1][1]);
    close(fd[2][1]);

    int x;
    printf("Input number: ");
    scanf("%d", &x);
    fflush(stdin);
    //primero enviamos el número al proceso secundario 1 mediante el pipe 0
    if (write(fd[0][1], &x, sizeof(int)) < 0) {
        return 8;
    }

    //leemos del pipe 2 el numero que nos manda el proceso secundario 2 (se quedará esperando en el read hasta que alguien -el proceso 2- escriba en ese punto de escritura de ese pipe en concreto -el pipe 2-)
    if (read(fd[2][0], &x, sizeof(int)) < 0) {
        return 9;
    }

    printf("Resultado: %d\n", x);
    //cerramos los file descriptors usados en el proceso padre
    close(fd[0][1]);
    close(fd[2][0]);

    //Esperamos por los dos procesos que creamos.
    waitpid(pid1, NULL, 0);
    waitpid(pid2, NULL, 0);

    return 0;
}
