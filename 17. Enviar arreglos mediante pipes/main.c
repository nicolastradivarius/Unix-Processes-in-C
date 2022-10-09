#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <sys/wait.h>
#include <time.h>

/**
 * Queremos:
 *  2 procesos,
 *      1) Que el proceso hijo genere números aleatorios y los envíe al proceso padre.
 *      2) El proceso padre los recibirá, los sumará a todos e imprimirá el resultado.
*/

int main(int argc, char const *argv[]) {
    int fd[2];
    //abrimos un pipe para enviar los datos
    if (pipe(fd) == -1) { return -2; }

    int pid = fork();

    if (pid == -1) { return -1; }

    if (pid == 0) {
        //Child process
        close(fd[0]); //cerramos el punto de lectura, porque el proceso hijo nunca lee nada, sólo manda datos.
        int n, i;
        int arr[10]; //el arreglo tendrá, como máximo, 10 elementos. Pero en realidad su cantidad es variable (con el valor de n).

        srand(time(NULL));
        n = rand() % 10 + 1; //calculamos la cantidad de elementos que habrá en nuestro arreglo.

        printf("Generated: ");
        for (i = 0; i < n; i++) {
            arr[i] = rand() % 11;
            printf("%d ", arr[i]);
        }
        printf("\n");

        //enviamos la cantidad de elementos al pipe, ya que de otro modo no tenemos conocimiento de cuántos elementos hay realmente en el arreglo (no podemos poner directamente 10 en el proceso padre)
        if (write(fd[1], &n, sizeof(int)) < 0) {
            return -4;
        }

        printf("Sent n = %d\n", n);

        if (write(fd[1], arr, sizeof(int) * n) < 0) {
            return -3;
        }

        printf("Sent array\n");

        close(fd[1]); //cerramos el punto de escritura, porque terminamos de escribir.
    } else {
        //Parent process
        close(fd[1]); //cerramos el punto de escritura, porque el proceso padre nunca escribe nada, sólo lee.

        int n, i, sum = 0;
        int arr[10];

        if (read(fd[0], &n, sizeof(int)) < 0) {
            return -5;
        }

        printf("Received n = %d\n", n);

        if (read(fd[0], arr, sizeof(int) * n) < 0) {
            return -6;
        }

        printf("Received array\n");

        close(fd[0]); //cerramos el punto de lectura porque terminamos de leer en el proceso padre.

        for (i = 0; i < n; i++) {
            sum += arr[i];
        }

        printf("Result is %d\n", sum);
        wait(NULL);
    }

    return 0;
}
