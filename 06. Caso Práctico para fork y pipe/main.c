#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include <errno.h>

int main(int argc, char const *argv[]) {
    //tenemos un array con multiples números, y queremos sumar todos los elementos de una forma diferente, tomando ventaja de tener diferentes procesos: dividimos el array en dos, dejando que una parte sea manipulada por un proceso, y la otra por el otro. Al final, ambos procesos tendrán una suma parcial, y un proceso escribirá en el pipe y el otro la leerá y la sumará.
    int arr[] = {1, 2, 3, 4, 1, 2};
    int arrSize = sizeof(arr) / sizeof(int); //esto nos da el número de elementos del array.
    int start, end;
    int fd[2];

    if (pipe(fd) == -1) {
        return -1;
    }

    int id = fork();
    if (id == -1) {
        return -1;
    }

    if (id == 0) {
        start = 0;
        end = start + arrSize/2; //se puede omitir el start.
    } else {
        start = arrSize/2;
        end = arrSize;
    }

    int sum = 0;
    int i;
    //cada proceso tiene un valor diferente de start!
    for (i = start; i < end; i++) {
        sum += arr[i];
    }

    printf("Calculando suma parcial: %d\n", sum);

    //Ahora vemos el tema del pipe y pasarle la suma parcial al padre.
    if (id == 0) {
        close(fd[0]); //lo cerramos porque no estamos leyendo nada
        write(fd[1], &sum, sizeof(sum));
        close(fd[1]);
    } else {
        int sumFromChild;
        close(fd[1]); //lo cerramos porque no estamos escribiendo nada
        read(fd[0], &sumFromChild, sizeof(sumFromChild)); //qué pasaría si el proceso padre es planificado antes que el hijo y llega a este punto antes de que el proceso hijo escriba en el pipe? El pipe estaría vacío, así que en cierta forma habría que esperar a que haya algo en el pipe para poder tomarlo. La función read() bloquea el proceso padre y esperará a que haya algo en el pipe para ser leído; la llamada a read() no retornará hasta que el proceso hijo llame a write().
        close(fd[0]);

        int totalSum = sum + sumFromChild;
        printf("Suma total es %d\n", totalSum);
        wait(NULL); //no hay garantía de que el proceso hijo haya sí o sí terminado después de escribir en el pipe (línea 44), así que por las dudas lo esperamos.
    }

    return 0;
}
