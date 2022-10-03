#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include <errno.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <fcntl.h>
#include <time.h>

//Crearemos dos programas: uno va a generar unos números, y el otro va a tomar esos números y mostrarlos por pantalla. El archivo FIFO lo creamos por consola.
int main(int argc, char const *argv[]) {
    //Generamos números aleatorios
    int arr[5];
    srand(time(NULL));
    int i;
    for (i = 0; i < 5; i++) {
        arr[i] = rand() % 10;
        printf("Generado %d\n", arr[i]);
    }

    int fd = open("sum", O_WRONLY);
    if (fd == -1) {
        printf("Hubo un error al querer abrir el archivo FIFO.\n");
        return -1;
    }

    
    //escribimos la información del arreglo arr en el archivo FIFO. Escribirá 5 integers uno detrás del otro en una sola llamada (será mucho más rápido que hacer el típico for de 5 vueltas y en cada iteración escribir un entero en el espacio de memoria del array)
    if (write(fd, arr, sizeof(int) * 5) == -1) {
        return 2;
    }
    close(fd);

    //Abrimos el archivo FIFO para lectura así poder obtener la suma e imprimirla por pantalla.
    fd = open("sum", O_RDONLY);
    if (fd == -1) {
        return -1;
    }

    int sum;
    if (read(fd, &sum, sizeof(sum)) == -1) {
        return -1;
    }
    close(fd);

    printf("Resultado es %d\n", sum);

    return 0;
}
