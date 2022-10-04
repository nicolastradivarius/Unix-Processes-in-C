#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h> 


int main(int argc, char const *argv[]) {
    //execl("/bin/ping", "/bin/ping", "google.com", NULL); //esta funcion toma un filename a un ejecutable y lo ejecuta con una serie de argumentos. Toma el nuevo proceso (memoria, etc) del ping y lo mete aquí, reemplazando absolutamente todo nuestro programa.
    //Y qué pasa si sólamente queremos hacer "ping" y ya, sin tanto lío con los parámetros de las funciones? Usamos execlp
    //execlp("ping", "ping", "google.com", NULL);

    //Con execvp podemos pasarle un array
    char* arr[] = {
        "ping",
        "google.com",
        NULL
    };
    //execvp("ping", arr);

    char* env[] = {
        "TEST=environment variable",
        NULL
    };
    //con execvpe podemos pasarle un environment, que es un set de variables que le podemos pasar a través de otro array
    execvpe("ping", arr, env);

    int err = errno;

    printf("Ping terminó su ejecución.\n");
    return 0;
}
