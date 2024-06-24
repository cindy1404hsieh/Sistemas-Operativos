#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <stdbool.h>

int main(int argc, char *argv[]) {
    

    if (argc != 2) {
        printf("Uso: %s <n>\n", argv[0]);
        exit(-1);
    }

    int n = atoi(argv[1]);

    if (n < 2) {
        printf("El valor de n debe ser mayor o igual a 2.\n");
        exit(-1);
    }

    int pipe_fds[2];
    pid_t pid;

    // Creamos el primer proceso
    if (pipe(pipe_fds) <0) {
        printf("error in pipe\n");
        exit(-1);
    }

    if ((pid = fork()) < 0) {
        printf("error in fork\n");
        exit(-1);
    }

    if (pid == 0) {
        // Este es el proceso hijo
        close(pipe_fds[1]); // Cerramos el extremo de escritura de la tubería

        while (true) {
            int primo;
            
            if (read(pipe_fds[0], &primo, sizeof primo) == 0) {
                break; // Si la tubería está cerrada, salimos
            }

            printf("primo %d\n", primo);

            int pipe_derecha[2];

            if (pipe(pipe_derecha) <0) {
                printf("error in pipe\n");
                exit(-1);
            }

            if ((pid = fork()) <0 ) {
                printf("error in fork\n");
                exit(-1);
            }

            if (pid == 0) {
                // Este es el proceso hijo, repetimos el proceso
                close(pipe_derecha[0]); // Cerramos el extremo de lectura de la tubería derecha

                while (true) {
                    int m;
                    
                    if (read(pipe_fds[0], &m, sizeof m) == 0) {
                        break; // Si la tubería está cerrada, salimos
                    }

                    if (m % primo != 0) {
                    	//printf("mi m vale:%d\n" , m);
                        write(pipe_derecha[1], &m, sizeof m);
                    }
                }

                close(pipe_derecha[1]); // Cerramos el extremo de escritura de la tubería derecha
                exit(0);
            } else {
                // Este es el proceso padre, cierra la tubería izquierda y continúa
                close(pipe_fds[0]); // Cerramos el extremo de lectura de la tubería izquierda
                pipe_fds[0] = pipe_derecha[0]; // Actualizamos la tubería izquierda
                close(pipe_derecha[1]); // Cerramos el extremo de escritura de la tubería derecha
            }
        }

        close(pipe_fds[0]);
        exit(0);
    } else {
        // Este es el proceso padre, escribe números del 2 a n en la tubería
        close(pipe_fds[0]); // Cerramos el extremo de lectura de la tubería

        for (int i = 2; i <= n; i++) {
            write(pipe_fds[1], &i, sizeof i);
        }
        close(pipe_fds[1]); // Cerramos el extremo de escritura de la tubería
        wait(NULL); // Esperamos a que el proceso hijo termine
    }

    return 0;
}




/*Llamadas al sistema: fork(2), pipe(2), wait(2).
La interfaz que se pide es:

$ ./primes <n> donde n será un número natural mayor o igual a 2. 

El código debe crear una estructura de procesos similar a la mostrada en la imagen, de tal manera que:

El primer proceso cree un proceso derecho, con el que se comunica mediante un pipe.

Ese primer proceso, escribe en el pipe la secuencia de números de 2 a n, para a continuación cerrar el pipe y esperar la finalización del proceso derecho.

Todos los procesos sucesivos son responsables de crear a su “hermano” derecho, y la tubería (pipe) que los comunica. aplican el pseudo-código mostrado a continuación:

p := <leer valor de pipe izquierdo>

imprimir p // asumiendo que es primo

mientras <pipe izquierdo no cerrado>:
    n = <leer siguiente valor de pipe izquierdo>
    si n % p != 0:
        escribir <n> en el pipe derecho

(El único proceso que es distinto, es el primero, que tiene que simplemente generar la secuencia de números naturales de 2 a n. No tiene lado izquierdo.)

Se debería poder ejecutar correctamente el programa con un N mayor o igual a 10000.

Ejemplo de uso:

$ ./primes 35
primo 2
primo 3
primo 5
primo 7
primo 11
primo 13
primo 17
primo 19
primo 23
primo 29
primo 31
*/

