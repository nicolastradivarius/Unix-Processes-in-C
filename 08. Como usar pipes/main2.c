#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include <errno.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <fcntl.h>

//Crearemos dos programas: uno va a generar unos números, y el otro va a tomar esos números y mostrarlos por pantalla
int main(int argc, char const *argv[]) {
    int arr[5];
    int fd = open("sum", O_RDONLY);
    if (fd == -1) {
        return -1;
    }

    int i;
    if (read(fd, arr, sizeof(int)*5) == -1) {
        return -1;
    }

    close(fd);

    int sum = 0;
    for (i = 0; i < 5; i++) {
        sum += arr[i];
    }
    
    //printf("Resultado es %d\n", sum);
    
    //Tarea: en lugar de imprimir la suma por pantalla, enviar la suma al otro programa y que éste la imprima.

    //Abrimos el archivo FIFO para escritura
    fd = open("sum", O_WRONLY);
    if (fd == -1) {
        return -1;
    }

    if (write(fd, &sum, sizeof(sum)) == -1) {
        return -1;
    }
    close(fd);

    return 0;
}
