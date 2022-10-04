#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include <signal.h>

//Las señales son para enviar pequeñas "notificaciones", enteros que dicen a un proceso qué debe hacer (como las señales del kill, por ejemplo).

int main(int argc, char const *argv[]) {
    int pid = fork();
    if (pid == -1) { return 1; }

    if (pid == 0) {
        while (1) {
            printf("Aquí va texto.\n");
            usleep(50000);
        }
    } else {
        sleep(1);
        kill(pid, SIGKILL);
        wait(NULL);
    }
    return 0;
}
