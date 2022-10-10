#define _XOPEN_SOURCE 700 //para evitar errores de subrayado
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <signal.h>
#include <sys/wait.h>

//Ahora la señal no terminará el proceso (comportamiento por defecto).
//Se supone que deben ser operaciones atómicas (que no consuman más de 1 ciclo de CPU, o bien, mucho tiempo). No deberían modificar variables globales, o imprimir por pantalla.
void handle_sigusr1(int signal) {
    printf("\n(HINT) Remember that multiplication is repetitive addition!\n");
}

/**
 * Problema: un programa hará una pregunta y el usuario tendrá que responderla. Si pasan más de 5 segundos y no hay respuesta, se enviará una pista al usuario. ¿Cómo lo hacemos con dos procesos?
 * */
int main(int argc, char const *argv[]) {
    int x;
    int answered = 0;

    int pid = fork();
    if (pid == -1) {return 1;}

    if (pid == 0) {
        sleep(5);
        //Una vez que pasen los 5 segundos, queremos enviarle una señal al proceso padre para que imprima una pista.
        kill(getppid(), SIGUSR1); //debemos modificar el comportamiento de la señal porque por defecto termina el proceso al cual se la envía.
    } else {
        struct sigaction sa = { 0 };
        sa.sa_flags = SA_RESTART; //sólo porque usamos scanf. Si no lo usáramos, no haría falt setear esta flag.
        sa.sa_handler = &handle_sigusr1;
        sigaction(SIGUSR1, &sa, NULL);
        
        printf("What is the result of 3 x 5: ");
        scanf("%d", &x);
        answered = 1;
        if (x == 15) {
            printf("Right!\n");
        } else {
            printf("Wrong!\n");
        }
        //No ponemos un WAIT porque eso nos genera que espere por el HINT aunque ya hayamos dado una respuesta. En el video pone a x como variable global y la analiza para, según su valor, imprimir la hint o no. Lo recomendable es tener otra variable que represente si el usuario respondió o no.
    }
    return 0;
}
