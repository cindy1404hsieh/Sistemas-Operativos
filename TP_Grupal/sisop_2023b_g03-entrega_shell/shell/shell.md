# shell

### Búsqueda en $PATH

**¿cuáles son las diferencias entre la syscall execve(2) y la familia de wrappers proporcionados por la librería estándar de C (libc) exec(3)?**

En su propósito general, la familia de exec(3) usan internamente la syscall execve(2) para reemplazar el actual programa en ejecución, por otro otro programa, manteniendo su proceso.

Respecto a sus diferencias:

familia exec(3):

- La mayoría de los de la familia (entre ellos execvp), no necesitan la proporción de rutas absolutas al ejecutable, lo que hace la interfaz de ellas más amigable. Es decir, la búsqueda se realiza diirectamente en la variable de entorno `$PATH`.

- sólo en los wrappers terminados con el caracter 'e' (como execvpe ó execle) se les puede pasar `envp`, en el resto de ellos, toma las variables de entorno definidas en el sistema, previas al llamado del wrapper.

execve(2):

- requiere que proporciones rutas absolutas de la dirección al ejecutable. 

- Te permite pasarle tus propias variables de entorno mediante el parámetro `envp`.

**¿Puede la llamada a exec(3) fallar? ¿Cómo se comporta la implementación de la shell en ese caso?**

La llamada a exec(3) puede fallar, en ese caso la función devuelve -1 y se asigna en `errno` el correspondiente tipo de error.

En nuestra implementación verificamos para el caso de que devuelva -1 y allí realizamos un `perror()` informando que hubo un error al ejecutar el binario.

---

### Procesos en segundo plano (Parte 1)


**Detallar cuál es el mecanismo utilizado para implementar procesos en segundo plano.**

A partir del struct cmd pasado en la funcion exec_cmd del archivo exec.c, al ser un proceso en background, lo primero que hicimos fue castearlo a un struct backcmd y su parametro backcmd->cmd a struct execcmd. Después ejecutamos el comando guardado por el struct execcmd mediante execvp.

Luego, en el archivo runcmd.c, en el caso de que el proceso sea del tipo BACK, imprimiamos la informacion de este en el proceso padre, y retornabamos para que vuelva a aparecer el prompt inmediatamente en la shell.

Además de esto, teníamos que usar un wait para esperar oportunamente a los procesos en segundo plano para que no queden procesos zombies. Para ello agregamos un nuevo waitpid configurado para que espere a cualquier proceso hijo de la terminal con el flag WNOHANG. Esto lo que permite es que al llamar a un nuevo comando, ese waitpid no sea bloqueante con el fin de avisarle al kernel para que libere los recursos de los procesos en segundo plano que hallan terminado anteriormente, antes de que aparezca el nuevo prompt.

---
### Procesos en segundo plano (Parte 5)

**Explicar detalladamente el mecanismo completo utilizado.**

Para lograr que el manejo y liberación de recursos del proceso, sea asincrónica, hicimos uso de señales.

Utilizamos la syscall sigaction(2) donde atrapamos y manejamos en un handler la señal SIGCHLD, que como vimos, indica que un proceso hijo ha terminado.

Para poder utilizar la syscall, tuvimos que definir un struct `sigaction` donde podemos definir distintas configuraciones sobre como se van a comportar las señales en distintas situaciones.

Entre las confguraciones que tuvimos en cuenta a la hora de que se produzcan las señales son:

- Definir flag SA_ONSTACK : para establecer un stack alternativo en donde se ejecuta el handler. Establecido previamente por `sigaltstack(2)`.

- Definir flag SA_RESTART: para en el caso de que la señal interrumpa la ejecución de una syscall, pueda reestablecer la ejecución de la misma.

- Definir handler: Es una función que le asignamos al struct `sigaction` para que al momento de producirse la señal/señales dada por `sigaction(2)` pueda ser manejada según nuestras necesidades.

- Definir con `sigfillset(&sa.sa_mask)`: Esto determina la forma en que se comporta el handler cuando le llegan distintas señales mientras se está ejecutando el mismo. Siendo `sa`, el struct `sigaction`.

Por otro lado, tuvimos que definir un stack alternativo de forma tal que el handler, pueda ser ejecutado en ese stack y no en el stack del programa en sí. Esto para evitar que en caso que el handler falle ó produzca un stack overflow, se corte la ejecución del programa principal.

Es por ello que definimos un struct `stack_t` y hacemos uso de la syscall `sigaltstack(2)` para determinar que tal struct sea nuestro stack alternativo.

Esto último combinado con el flag de `sigaction` SA_ONSTACK, produce que el handler se ejecute dentro de ese `stack_t` previamente definido.

Por otro lado, con el objetivo de capturar sólo las señales de comandos `BACKGROUND`, tuvimos que utilizar el concepto de `pgid` (proccess group id), es decir de agrupar de alguna forma a los procesos de tipo BACKGROUND, de tal forma que al momento de producirse una señal SIGCHLD, el manejador sepa que la señal es producida por un comando de BACKGROUND, y no otro tipo.

Esto lo logramos, seteando al pid del padre como el `pgid` del hijo de tipo BACKGROUND.

Además, para que el handler sólo imprima procesos en segundo plano terminados, tuvimos que establecer un `pgid` para los demás tipo de comandos, en nuestro caso hicimos que el `pgid` del proceso hijo en ejecución sea el mismo que el `pid` persé.

Por último, defimos el handler en donde para realizar la liberación de recursos utilizamos: 

```
waitpid(0, &status, WNOHANG);
```

el resultado de llamar a `waitpid` con esos argumentos, y con los pgid de los procesos hijos seteados devuelve:

- `mayor a 0`: si la señal es dada por un proceso background.(Ej: el pid del proceso en segundo plano concluido)
- `0`: si por ejemplo, termina un proceso en primer plano y hay otro en segundo plano.
- `-1`: Error ó en otro caso (Ej: Ejecutando un proceso en primer plano)

Finalmente imprimimos convenientemente el PID del proceso background que terminó.

**¿Por qué es necesario el uso de señales?**

Las señales permiten reportar errores o determinados eventos que pueden pasar en un programa. Hay algunas señales que terminan con la ejecución del programa de forma inmediata y otras que no.
    
En caso de ser necesario, lo que nos permite el uso de señales es la posibilidad definir un handler para indicarle al sistema operativo como debe tratar con algun tipo de señal de una forma que especifica. En nuestro caso, nosotros debemos manejar mediante un hadler la señal de SIGCHLD, la cual nos indica que un proceso hijo ha terminado, para poder liberar sus recursos de forma inmediata y evitar tener procesos zombies.

---

### Flujo estándar

--- 
1. **Investigar el significado de 2>&1, explicar cómo funciona su forma general.**
2. **Mostrar qué sucede con la salida de cat out.txt.**
3. **Luego repetirlo, invirtiendo el orden de las redirecciones (es decir, 2>&1 >out.txt). ¿Cambió algo? Compararlo con el comportamiento en bash(1).**
---
1.
El comando "2>&1" se utiliza en sistemas Unix y Unix-like para redirigir la salida estándar de error (stderr) hacia la misma ubicación que la salida estándar (stdout). En otras palabras, este comando combina la salida estándar y la salida de error y las redirige juntas a un archivo o a la salida estándar.

2. 
``` 
$ ls -C /home /noexiste >out.txt 2>&1
$ cat out.txt

ls: no se puede acceder a '/noexiste': No existe el archivo o el directorio
/home:
cindy
```

3.
```
$ ls -C /home /noexiste 2>&1 >out.txt
ls: no se puede acceder a '/noexiste': No existe el archivo o el directorio

$ cat out.txt
/home:
cindy
```

En el primer conjunto de comandos:

El comando ls -C /home /noexiste intenta listar el contenido de los directorios /home y /noexiste. Luego, redirige la salida estándar (stdout) de este comando al archivo out.txt y, mediante 2>&1, también redirige la salida de error estándar (stderr) al mismo archivo out.txt. Como /noexiste no existe, se genera un mensaje de error, que se almacena en out.txt.

En el segundo conjunto de comandos:

El comando ls -C /home /noexiste también intenta listar el contenido de /home y /noexiste. Sin embargo, en este caso, primero redirige la salida de error estándar (stderr) al mismo lugar que la salida estándar (stdout), que es out.txt. Luego, redirige la salida estándar (stdout) a out.txt. Esto significa que ambos, la salida estándar y la salida de error, inicialmente se redirigen a out.txt, pero la redirección de la salida estándar ocurre después de la de la salida de error.

Debido a este orden de redirección, el contenido de out.txt contendrá solo la salida estándar del comando ls, que es la lista de contenido de /home. La salida de error no se almacenará en out.txt.

El orden de las redirecciones es importante:

2>&1 redirige la salida de error estándar (stderr) hacia el mismo destino que la salida estándar (stdout). En este caso, ambos se dirigen a la terminal, por lo que veremos el mensaje de error en la pantalla.

Luego, >out.txt redirige la salida estándar (stdout) hacia el archivo out.txt. Sin embargo, la salida de error ya se redirigió a la terminal en el paso anterior, por lo que solo la salida estándar se almacena en out.txt.

Por lo tanto, cuando ejecutamos estos comandos en la terminal, primero vemos el mensaje de error en la pantalla debido a la redirección de la salida de error a la terminal, y luego, cuando ejecutas cat out.txt, solo vemos la salida estándar que se almacenó en out.txt, que es la lista de contenido de /home.

---
### Tuberías múltiples
**Investigar qué ocurre con el exit code reportado por la shell si se ejecuta un pipe
¿Cambia en algo?
¿Qué ocurre si, en un pipe, alguno de los comandos falla? Mostrar evidencia (e.g. salidas de terminal) de este comportamiento usando bash. Comparar con su implementación.**

El exit code si se ejecuta un pipe es el del último comando ejecutado. Se muestra el exit code del último comando ejecutado sin importar los anteriores. En nuestra implementación pasa lo mismo.

Bash:

    ignaciodematias@LAPTOP-99BMDOFD:~$ grep D <noexiste | ls
    -bash: noexiste: No such file or directory
    Android  Desktop  err.txt  out.txt  out1.txt  out2.txt  snap
    ignaciodematias@LAPTOP-99BMDOFD:~$ echo $?
    0

Nuestra implementación:

      (/home/ignaciodematias)
    $ grep D <noexiste | ls
    sh: noexiste: No such file or directory
    Android  Desktop  err.txt  out.txt  out1.txt  out2.txt  snap
     (/home/ignaciodematias)
    $ echo $?
    0

---

### Variables de entorno temporarias

**¿Por qué es necesario hacerlo luego de la llamada a fork(2)?**

Es necesario hacer el seteo de variables luego del fork(2) debido a que, al ser temporales, lo que buscamos es que estas variables dejen de existir en el entorno de variables, luego de la ejecución de un comando cualquiera.

En otras palabras, al realizar un fork(2), el mismo incluye una copia de variables de entorno que se hayan definido hasta ese momento. 

Es por ello, que en el caso de setear las variables antes de realizar el fork, las variables temporales perdurarían por fuera de la ejecución del comando sobre el cual querríamos aplicarlo.

En contraposición, si las variables son seteadas luego de la llamada a fork(2), en particular, dentro del hijo, las variables definidas, serán utilizadas sólo en la ejecución del comando en particular.

**En algunos de los wrappers de la familia de funciones de exec(3) (las que finalizan con la letra e), se les puede pasar un tercer argumento (o una lista de argumentos dependiendo del caso), con nuevas variables de entorno para la ejecución de ese proceso. Supongamos, entonces, que en vez de utilizar setenv(3) por cada una de las variables, se guardan en un arreglo y se lo coloca en el tercer argumento de una de las funciones de exec(3).**

- **¿El comportamiento resultante es el mismo que en el primer caso? Explicar qué sucede y por qué.**

    El comportamiento no es el mismo.

    Esto es así, dado que las variables de entorno que se envían a través del tercer parametro, serán las únicas que se tendrán en cuenta a la hora de correr el binario.

    Esto es distinto a lo que tenemos en nuestra implementación, debido a que nosotros hacemos uso de `setenv(3)`, el cual modifica el entorno de variables que se encuentran en el sistema (la variable externa `environ`) y por ende, al ejecutar un `execvp`, además de tomar las variables temporales definidas, se tendrán en cuenta las variables que existen en el entorno del sistema y que no hayan sido sobreescritas. (ej: en entornos linux, las variables de entorno del sistema se pueden ver en `/usr/bin/env`).

- **Describir brevemente (sin implementar) una posible implementación para que el comportamiento sea el mismo.**

    Una posible implementación en nuestra shell para ejecutar binarios con comandos `exec(3)`, que tienen el sufijo 'e', puede ser simplemente utilizar la función set_environ_vars ya definida en el tp y luego pasarle, como tercer parámetro, la variable externa `extern char* environ` (casteada de forma tal que compile) a la función de exec(3) con sufijo 'e' que utilicemos, para que tome ese entorno modificado por set_environ_vars.

    Como utilizamos el set_environ_vars dentro del hijo sobre el cual se ejecuta el binario (comando), `setenv(3)`, internamente modifica la variable externa `environ` del proceso "hijo" y luego de ejecutar el proceso "hijo", el proceso "padre" dejará de tener las variables modificadas por set_environ_vars y por ende tendrá el mismo comportamiento que la implementación actual.


### Pseudo-variables

**Investigar al menos otras tres variables mágicas estándar, y describir su propósito.
Incluir un ejemplo de su uso en bash (u otra terminal similar).**

Las variables mágicas o pseudo-variables son variables especiales en las shells de Unix/Linux que proporcionan información útil sobre el entorno o el proceso en ejecución. Aca hay tres ejemplos con sus propósitos y ejemplos de uso en la shell Bash:

1.$# (Número de argumentos):

Propósito: Almacena el número de argumentos pasados al script o comando.

Ejemplo de uso:

#!/bin/bash
echo "Número de argumentos pasados: $#"

Ejecución:
$ ./mi_script.sh arg1 arg2 arg3
Número de argumentos pasados: 3

2.$0 (Nombre del script o comando):

Propósito: Almacena el nombre del script o comando actual.

Ejemplo de uso:
#!/bin/bash
echo "Nombre del script actual: $0"

Ejecución:
$ ./mi_script.sh
Nombre del script actual: ./mi_script.sh

3.$? (Código de salida del último comando):

Propósito: Almacena el código de salida (exit status) del último comando ejecutado. Un valor de 0 indica éxito, mientras que un valor diferente de 0 indica un error.

Ejemplo de uso:
#!/bin/bash
ls archivo_inexistente.txt
if [ $? -ne 0 ]; then
    echo "Error: El comando ls falló."
else
    echo "El comando ls se ejecutó con éxito."
fi
Ejecución:

$ ./mi_script.sh
ls: archivo_inexistente.txt: No such file or directory
Error: El comando ls falló.

---
### Comandos built-in

**¿Entre cd y pwd, alguno de los dos se podría implementar sin necesidad de ser built-in? ¿Por qué? ¿Si la respuesta es sí, cuál es el motivo, entonces, de hacerlo como built-in?**

El comando pwd puede ser implementado sin necesidad de ser built-in. La decisión de hacer un comando "built-in" como pwd en una shell generalmente se basa en consideraciones de eficiencia y rendimiento. Si pwd fuera implementado como un comando externo, cada vez que se llamara a pwd, se crearía un nuevo proceso mediante llamadas al sistema, como fork y exec, para ejecutar el comando y obtener el resultado. Esto puede ser ineficiente, especialmente para comandos simples que no requieren la creación de un nuevo proceso.

---

### Historial

---
