#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include <time.h>

//creamos un programa que genera un numero del 1 al 5, lo envía a otro proceso, este otro proceso lo multiplica por 4, y lo envía devuelta al proceso original. Ej: 5 => 5 * 4 = 20 => 20
int main(int argc, char const *argv[]) {
    //p1 es el pipe que viene del child al parent, y p2 es el que viene del parent al child
    int p1[2], p2[2];
    if (pipe(p1) == -1) { return -1; }
    if (pipe(p2) == -1) { return -1; }

    int pid = fork();
    if (pid == -1) { return -1; }

    if (pid == 0) {
        close(p1[0]);
        close(p2[1]);
        //proceso hijo
        int x;
        if (read(p2[0], &x, sizeof(x)) == -1) { return -1; } //leo la data que me envió el padre por el pipe2...
        printf("Recibido %d\n", x);
        x *= 4;
        if (write(p1[1], &x, sizeof(x)) == -1) { return -1; } //y escribo la data en el pipe1 (para que el padre la pueda leer)
        printf("Escribí %d\n", x);
        close(p1[1]);
        close(p2[0]);
    } else {
        //proceso padre
        close(p1[1]);
        close(p2[0]);
        srand(time(NULL));
        int y = rand() % 10;
        printf("Escribo %d...\n", y);
        if (write(p2[1], &y, sizeof(y)) == -1) { return -1; } //escribo en un punto del pipe2...
        if (read(p1[0], &y, sizeof(y)) == -1) { return -1; } //y me quedo bloqueado en el pipe1 esperando para leer la data... (el hijo me enviará la data)
        printf("Resultado es %d\n", y);
        close(p1[0]);
        close(p2[1]);
        wait(NULL); //hago un wait para proseguir una vez que el proceso hijo haya efectivamente terminado.
    }
    
    //El problema que teníamos era que usábamos un solo pipe, y cuando el proceso padre escribía información en él, inmediatamente la tomaba y la procesaba, lo cual dejaba al proceso hijo sin sentido. Ahora usamos dos pipes, cerramos los puntos que no nos interesan y solo usamos cada pipe con el propósito de lograr que el proceso padre escriba en el pipe que va a ser usado para enviar la información al hijo, y se quede esperando por información en el otro pipe, que es por donde el hijo le va a mandar la información. De esta forma, el pipe no es utilizado para lectura y escritura al mismo tiempo..

    return 0;
}
