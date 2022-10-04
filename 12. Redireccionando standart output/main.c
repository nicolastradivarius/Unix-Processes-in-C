#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include <fcntl.h>

int main(int argc, char const *argv[]) {
    int pid = fork();
    if (pid == -1) { return -1; }

    //Si llamamos a execlp en el proceso padre, éste es reemplazado y no podrá esperar por el proceso hijo (el hijo quedará zombie). Así que no se puede llamar execlp en el proceso padre, sólo al hijo, y especificar que el padre debe esperar a que el hijo finalice.

    if (pid == 0) {
        //proceso hijo

        //queremos que los resultados del programa "ping" vayan a parar a este archivo
        int file = open("pingResults.txt", O_WRONLY | O_CREAT, 0777);
        if (file == -1) { return 2; }

        //Los file descriptors son numeros que se refieren a un file. Por ejemplo, 0 se refiere a standard input, 1 a standard output, 2 a standard error. Así, hacer read(0) == scanf(), write(1, ...) se mostrará por pantalla. Cuando creamos el archivo file con open, éste nos devolvió un file descriptor. Así, podemos pensar que el file descriptor 3 se refiere a pingResults.txt. Entonces, lo que queremos es que la información que el programa ping muestra por la terminal la mande al archivo txt (cambiar el 1 por el 3). Lo hacemos con dup()
        
        //int file2 = dup(file); //nos retorna otro file descriptor para nuestro archivo (ahora tenemos dos file descriptor, 3 y 4, para pingResults.txt). Nos sirve pero no del todo...
        printf("El fd a pingResults.txt: %d\n", file);
        int file2 = dup2(file, STDOUT_FILENO); //toma 2 parámetros: el primero es el file descriptor que queremos clonar, y el segundo es el valor que queremos que el nuevo file descriptor tenga. Ahora tenemos el file descriptor de 3, y el de valor 1 que reemplazó a standard output por pingResults.txt.  Ahora cuando el programa ping quiera imprimir algo por pantalla (como los resultados del ping), éstos irán al archivo .txt en lugar de mostrarse por pantalla.
        close(file); //cerramos el file descriptor original porque no lo necesitamos.

        int status = execlp("ping", "ping", "-c", "3", "google.com", NULL);
        //execlp("mkfifo", "mkfifo", "sum", NULL);
        //cualquier cosa que pongamos después del execlp no importará porque será reemplazado. Eso hace que perdamos el control de la programación de nuestro proceso hijo.
        //Cómo chequeamos si el programa que llamamos tuvo un error o no existe? execlp sólo reemplazará la memoria del proceso sólo si tiene éxito, si hay un error esto no ocurrirá y simplemente seguirá con la ejecución de nuestro programa.
        if (status == -1) { printf("No se pudo encontrar el programa a ejecutar.\n"); }
    } else {
        //proceso padre
        int waitStatus;
        wait(&waitStatus);
        if (WIFEXITED(waitStatus)) { //si el proceso por el cual esperamos terminó...
            int statusCode = WEXITSTATUS(waitStatus); //statusCode es el valor de retorno del programa que ejecutamos.
            if (statusCode == 0){
                printf("Éxito!\n");
            } else {
                printf("Falló con el código %d!\n", statusCode);
            }
        }
        //Cómo hacemos para saber si el programa que ejecutó y reemplazó al proceso hijo tuvo éxito? 
        printf("Algo de procesamiento va aquí!\n");
    }
    return 0;
}

