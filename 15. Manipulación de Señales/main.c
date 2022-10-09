#define _XOPEN_SOURCE 700 //para evitar errores de subrayado
#include <stdlib.h>
#include <stdio.h>
#include <signal.h>

//Cuando usamos CTRL+Z
void handle_sigtstp(int signal) {
    printf("Stop not allowed!\n");
}

//Cuando usamos "fg".
//A pesar de estar modificando el comportamiento de la señal, igualmente va a seguir teniendo el comportamiento de traer el proceso de segundo plano y continuar su ejecución, solo que a eso le agrega las líneas que escribimos aquí, a diferencia del manejo de SIGTSTP que lo "reemplazaba".
void handle_sigcont(int signal) {
    printf("Input number: ");
    fflush(stdout);
}

int main(int argc, char const *argv[]) {
    //Dentro de este struct, contenemos la información acerca de cómo manejar las señales.
    struct sigaction sa;
    //Seteamos la función que va a ser llamada cuando sea que recibamos una cierta señal.
    sa.sa_handler = &handle_sigcont;
    sa.sa_flags = SA_RESTART; //proveemos esta flag porque estamos usando scanf
    sigaction(SIGCONT, &sa, NULL);

    //signal(SIGTSTP, &handle_sigtstp); //Deprecado

    int x;
    printf("Input number: ");
    scanf("%d", &x);
    printf("Result: %d * 5 = %d\n", x, x*5);
    return 0;
}
