#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <sys/wait.h>
#include <time.h>
#include <string.h>

#define PROCESS_NUM 30

/**
 * Problema: un proceso principal genera un número y lo manda a través de toda una serie de procesos secundarios que le van sumando 1 (ver esquema). El número de procesos es variable, con lo cual el resultado depende de la cantidad de procesos que definamos.
*/

int main(int argc, char const *argv[]) {
    int pids[PROCESS_NUM];
    int pipes[PROCESS_NUM + 1][2];
    int i;
    for (i = 0; i < PROCESS_NUM + 1; i++) {
        if (pipe(pipes[i]) == -1) {
            perror("Error with creating pipe\n");
            return 1;
        }
    }

    for (i = 0; i < PROCESS_NUM; i++) {
        pids[i] = fork();
        if (pids[i] == -1) {
            perror("Error with creating process\n");
            return 2;
        }

        if (pids[i] == 0) {
            // Child process
            //salimos del loop con lo cual cada proceso hijo no vuelve a iterar y forkear devuelta.
            //break;
            int j;
            for (j = 0; j < PROCESS_NUM + 1; j++) {
                //cerramos los puntos de lectura de los pipes que se clonaron en cada proceso forkeado
                if (i != j) { //si el index del proceso es igual al index del pipe, entonces queremos leer de ese pipe, con lo cual NO debemos cerrarlo.
                    close(pipes[j][0]);
                }
                if (i + 1 != j) { //si el index del proceso + 1 es igual al index del pipe, entonces queremos escribir en ese pipe para que ese proceso pueda leer después, con lo cual NO debemos cerrarlo.
                    close(pipes[j][1]);
                }
            }

            int x;
            //sabemos que el pipe del que queremos leer tiene el mismo index que nuestro proceso actual
            if (read(pipes[i][0], &x, sizeof(int)) == -1) {
                perror("Error al leer\n");
                return 3;
            }

            printf("(%d) Got %d\n", i, x);
            x++;

            if (write(pipes[i+1][1], &x, sizeof(int)) == -1) {
                perror("Error al escribir\n");
                return 4;
            }

            printf("(%d) Sent %d\n", i, x);


            //cerramos los fd que usamos
            close(pipes[i][0]);
            close(pipes[i+1][1]);
            
            return 0;
        }
    }

    // Main process

    //Cerramos los otros pipes que el proceso main no usa.
    int j;
    for (j = 0; j < PROCESS_NUM + 1; j++) {
        //cerramos los puntos de lectura de los otros pipes.
        if (j != PROCESS_NUM) { //si es el último pipe, entonces no lo cerramos porque es el pipe del cual leemos.
            close(pipes[j][0]);
        }
        //cerramos los puntos de escritura de los otros pipes.
        if (j != 0) { //si el pipe es el primero, entonces no lo cerramos porque es el pipe en el que escribimos.
            close(pipes[j][1]);
        }
    }

    int y = 5;

    printf("El proceso principal envió: %d\n", y);


    if (write(pipes[0][1], &y, sizeof(int)) == -1) {
        perror("Error al escribir\n");
        return 5;
    }

    if (read(pipes[PROCESS_NUM][0], &y, sizeof(int)) == -1) {
        perror("Error al leer\n");
        return 6;
    }

    printf("El resultado final es: %d\n", y);

    //cerramos los pipes que usamos en el proceso main.
    close(pipes[0][1]);
    close(pipes[PROCESS_NUM][0]);

    for (i = 0; i < PROCESS_NUM; i++) {
        wait(NULL);
    }

    return 0;
}
