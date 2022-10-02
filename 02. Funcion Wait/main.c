#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <time.h>

//Esperar por procesos a terminar
//Queremos hacer un sollo fork, e imprimir números del 1 al 10 en pantalla.
//Los primeros 5 los va a imprimir el proceso hijo, y los siguientes 5 los va a imprimir el proceso padre.
int main(int argc, char const *argv[])
{
    int id = fork();
    //numero del que partimos: del padre, empieza por 6. Del hijo, por 0
    int n;
    if (id == 0) {
        n = 1;
    }
    else {
        n = 6;
    }

    //si estamos en el proceso padre, esperamos porque termine el proceso hijo para continuar. Especificamos esto porque sino también lo haría el proceso hijo
    if (id != 0) {
        wait();
    }

    int i;
    for (i = n; i < n + 5; i++) {
        printf("%d ", i);
        fflush(stdout);
    }

    printf("\n");
    
    //Esto tiene un problema y es que, sin un wait(), la impresión de los números se hace completamente al azar: el sistema operativo decide qué proceso imprime antes que el otro, y luego puede saltar al hijo, etc. La ejecución del programa se termina volviendo caótica y es por eso que necesitamos indicarle al proceso padre que espere por el hijo (por algún hijo, no especificamos cuál).
    return 0;
}
