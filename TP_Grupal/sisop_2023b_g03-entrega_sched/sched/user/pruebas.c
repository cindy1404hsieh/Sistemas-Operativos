// hello, world
#include <inc/lib.h>

int pruebas_exitosas = 0;
int pruebas_fallidas = 0;
int pruebas_totales = 0;

char *
chequeo_pruebas(int booleano)
{
	pruebas_totales++;
	if (booleano) {
		pruebas_exitosas++;
		return "OK";
	} else {
		pruebas_fallidas++;
		return "FAIL";
	}
}

void
umain(int argc, char **argv)
{
	cprintf("********* INICIO PRUEBAS SCHEDULER CON PRIORIDADES "
	        "*********\n");
	int prior_padre = 0;
	int prior_hijo = 0;
	prior_padre = thisenv->priority;
	int id;
	if ((id = fork()) < 0) {
		cprintf("error in fork\n");
		return;
	}
	if (id == 0) {
		prior_hijo = thisenv->priority;
		cprintf("Soy el proceso hijo %d, mi padre es %d y mi prioridad "
		        "es: %d \n",
		        thisenv->env_id,
		        thisenv->env_parent_id,
		        thisenv->priority);
		return;
	}
	// espera a que termine el hijo
	for (int i = 0; i < 1000000; i++)
		;

	cprintf("Prioridad hijo es: %d, prioridad padre es: %d\n",
	        prior_hijo,
	        prior_padre);
	cprintf("	La prioridad del proceso hijo es menor que la del "
	        "padre %s\n",
	        chequeo_pruebas(prior_hijo > prior_padre));


	cprintf("\n\n********* Pruebas totales: %d, exitosas: %d, fallidas: %d "
	        "********* \n",
	        pruebas_totales,
	        pruebas_exitosas,
	        pruebas_fallidas);
}