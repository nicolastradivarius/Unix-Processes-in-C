#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include <errno.h>

int main(int argc, char const *argv[])
{
    //Hay una funcionalidad en C que se llama pipe. Un pipe es como una especie de búfer en memoria del cual se puede leer y se puede escribir
    int fd[2];
    //fd[0] es el punto para leer - read
    //fd[1] es el punto para escribir - write
    if (pipe(fd) == -1) {
        printf("Un error ocurrió con la apertura del pipe");
        return -1;
    }


    //lo hacemos en este orden ya que al clonar el proceso, los file descriptors se copian de manera inteligente, permitiendo que ambos procesos se comporten de manera independiente (de modo que si uno cierra el pipe, en el otro queda abierto).

    //Vamos a crear un programa que pide un número al usuario, y luego lo envía mediante el pipe al proceso padre, y éste lo imprime por pantalla.
    int id = fork();
    if (id == -1) {
        printf("Un error ocurrió al intentar crear un nuevo proceso.\n");
        return -1;
    }
    if (id == 0) {
        //Lo cerramos porque nunca leemos del pipe en este proceso.
        close(fd[0]);
        int x;
        printf("Digita un número: ");
        scanf("%d", &x);
        if (write(fd[1], &x, sizeof(int)) == -1) {
            printf("Un error ocurrió al intentar escribir en el pipe.\n");
            return 2;
        }
        //cerramos el punto de escritura del pipe.
        close(fd[1]);
    } else {
        close(fd[1]); //cerramos el punto de escritura del pipe porque nunca escribimos el pipe en este proceso.
        int y;
        if (read(fd[0], &y, sizeof(int)) == -1) {
            printf("Un error ocurrió al intentar leer del pipe.\n");
            return 3;
        }
        close(fd[0]);
        printf("Tomamos del proceso hijo: %d\n", y);
    }

    return 0;
}
