#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include <signal.h>

int main(int argc, char const *argv[]) {
    int x;
    printf("Input number: ");
    scanf("%d", &x);
    printf("Result: %d * 5 = %d\n", x, x*5);
    return 0;
}

/*
 Mientras el programa está ejecutando, cuando llega a la línea de scanf(), se queda esperando por nuestro input. Si apretamos CTRL+Z en la terminal, le enviaremos al proceso una señal de SIGTSTP (que es básicamente SIGSTOP) para DETENER el proceso (no terminarlo, solo detenerlo, puesto que si se lo terminara, toda la información en memoria se liberaría). Luego, podemos enviarle una señal de SIGCONT mediante el comando "fg" en la terminal, que lo trae y continúa su ejecución en el punto donde se detuvo.
*/