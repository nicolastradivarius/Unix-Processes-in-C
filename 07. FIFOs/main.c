#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include <errno.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <fcntl.h>

int main(int argc, char const *argv[]) {
    //La comunicación con pipe entre dos procesos que no están en la misma jerarquía no se puede hacer. En lugar de ello, se usan las FIFOs files (unnamed pipes).
    if (mkfifo("myfifo1", 0777) == -1) {
         //0777 es: read-from, wrote-from por cualquiera
        if (errno != EEXIST) {
            printf("No se pudo crear la FIFO file.\n");
            return -1;
        }
    }
    printf("Abriendo el FIFO...\n");
    int fd = open("myfifo1", O_WRONLY); //si abrimos un fifo para escritura, el proceso que lo abre se bloquea hasta que otro proceso viene y abre el mismo FIFO para lectura... cuando esto sucede, el proceso que lo abrió para escritura continuará su ejecución. Podemos abrir otra terminal y hacer un "cat myfifo1" para leer el contenido del archivo, de esa forma veremos cómo la terminal que se quedó bloqueada continúa su ejecución.

    if (fd == -1) {
        printf("Error al abrir el FIFO!");
    }
    printf("Abierto...\n");
    int x = 97;
    if (write(fd, &x, sizeof(x)) == -1) {
        printf("Ocurrió un error.\n");
        return -1;
    }
    printf("Escrito...\n");
    close(fd);
    printf("Cerrado...\n");

    return 0;
}
