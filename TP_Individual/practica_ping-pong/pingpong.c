#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <sys/types.h>
#include <unistd.h>
#include <sys/wait.h>

#define READ 0 // file descriptor de lectura
#define WRITE 1 // file descriptor de escritura

int main(void)
{
	srandom(time(NULL));
	long int r = random();
	pid_t parent_id = getpid(); 

	int parent_child_fds[2];
	int child_parent_fds[2];

	// creo dos pipes  
	if(pipe(parent_child_fds) < 0 ){ //-1, error
		printf("error in pipe\n");
		exit(-1);
	}
	if(pipe(child_parent_fds) < 0 ){ //-1, error
		printf("error in pipe\n");
		exit(-1);
	}


	printf("Hola, soy PID <%d>\n", parent_id);
	printf("-IDs del primer pipe:[%d , %d]\n", parent_child_fds[0], parent_child_fds[1]);
	printf("-IDs del segundo pipe:[%d , %d]\n", child_parent_fds[0], child_parent_fds[1]);

	// fork
	pid_t child_id = fork();
	if(child_id < 0){
		printf("error in fork\n");
		exit(-1);
	}

	//ACA
	if(child_id == 0){ 
		//HIJO
		long int value;

		close(parent_child_fds[1]);//no necesito escribir lo de mi papa con este pipe
		close(child_parent_fds[0]);//no necesito leer lo de mi papa con este pipe

		read(parent_child_fds[0], &value , sizeof value); //no va a ejecutar hasta que hay algo que leer.
		//read(donde leo , &me lo guardo en este variable , tamanio del variable)
		printf("ACA ES EL HIJO (==0):\n");
		printf("- getpid me devuelve:<%d>(el pid de el mismo)\n",getpid() );
		printf("- getppid me devuelve:<%d>(el pid de su padre)\n",getppid() );
		printf("- recibo valor <%ld> via fd= %d\n", value, parent_child_fds[0] );
		printf("- reenvio valor en fd= %d y termino\n", child_parent_fds[1] );

		write(child_parent_fds[1], &value ,sizeof value);
		//write(donde escribo , &valor , tamanio del valor)
		close(child_parent_fds[1]);
		close(parent_child_fds[0]);

	}else{
		//PADRE
		long int recv_value; // variable que guarda lo que me reenvia el hijo
		close(parent_child_fds[0]);
		close(child_parent_fds[1]);

		printf("ACA ES EL PADRE (!=0):\n");
		printf("- getpid me devuelve:<%d>(el pid de el mismo)\n",getpid() );
		printf("- getppid me devuelve:<%d>(el pid de su padre)\n",getppid() );

		printf("- valor random: <%ld> \n", r);
		printf("- envio valor <%ld> a traves de fd= %d\n", r, parent_child_fds[1] );
		
		write(parent_child_fds[1], &r , sizeof r);
		read(child_parent_fds[0], &recv_value , sizeof recv_value);

		wait(NULL); //para que cierre el proceso hijo por completo

		printf("Hola, de nuevo soy PID: <%d>\n", parent_id);
		printf("- recibi valor <%ld> via fd=%d\n", recv_value, child_parent_fds[0]);

		close(child_parent_fds[0]);
		close(parent_child_fds[1]);

	}

	return 0;
}