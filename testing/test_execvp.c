#include <stdio.h>
#include <unistd.h>
#include <sys/wait.h>
#include <string.h>

int main() {
    // Arreglo de argumentos para ejecutar "ls -l"
    char *args[] = {"ls", "-l", NULL};  // El último elemento es NULL

    // Crear un proceso hijo
    pid_t pid = fork();

    if (pid == 0) {
        // Proceso hijo: ejecutar el comando usando execvp
        if (execvp(args[0], args) == -1) {
            perror("Error al ejecutar execvp");
        }
        return 1;  // Si execvp falla
    } else if (pid > 0) {
        // Proceso padre: esperar a que el hijo termine
        wait(NULL);
    } else {
        perror("Error al crear el proceso");
    }

    return 0;
}
